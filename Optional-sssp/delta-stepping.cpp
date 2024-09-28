#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include <random>

#include <sycl/sycl.hpp>

class relax_light_edges;
class relax_heavy_edges;

constexpr int INF = std::numeric_limits<int>::max();


struct Edge {
    int src, dest, weight;
};

void print_distances(const std::vector<int>& dist) {
    for (int i = 0; i < dist.size(); ++i) {
        if (dist[i] == INF)
            std::cout << "Vertex " << char('A' + i) << " is unreachable from source\n";
        else
            std::cout << "Distance from source to vertex " << char('A' + i) << " is " << dist[i] << "\n";
    }
}

void print_buckets(const std::vector<std::vector<int>>& buckets) {
    for (int i = 0; i < buckets.size(); ++i) {
        if (!buckets[i].empty()) {
            std::cout << "Bucket " << i << ": ";
            for (int v : buckets[i]) {
                std::cout << char('A' + v) << " ";
            }
            std::cout << "\n";
        }
    }
}


void print_graph(const std::vector<Edge>& edges) {
    if (edges.empty()) { std::cout << "Graph is empty\n"; return; }
    for (const auto& edge : edges) {
        std::cout << "src: " << edge.src << " dest: " << edge.dest << " weight: " << edge.weight << "\n";
    }
    std::cout << std::endl;
}

void deltaStepping(const std::vector<Edge>& edges, int src, int V, int E, int DELTA) {
    // Flattened graph representation -> Unable to use complex objects in vectors for kernel parallel execution 
    std::vector<int> edge_src(E);
    std::vector<int> edge_dest(E);
    std::vector<int> edge_weight(E);
    for (int i = 0; i < E; i++) {
        edge_src[i] = edges[i].src;
        edge_dest[i] = edges[i].dest;
        edge_weight[i] = edges[i].weight;
    }

    std::vector<int> dist(V, INF);
    dist[src] = 0;

    sycl::queue queue;
    std::cout << "Running on "
              << queue.get_device().get_info<sycl::info::device::name>()
              << "\n";

    std::vector<std::vector<int>> buckets((INF / DELTA) + 1);
    buckets[0].push_back(src);

    sycl::buffer<int, 1> edge_src_buf(edge_src.data(), edge_src.size());
    sycl::buffer<int, 1> edge_dest_buf(edge_dest.data(), edge_dest.size());
    sycl::buffer<int, 1> edge_weight_buf(edge_weight.data(), edge_weight.size());
    sycl::buffer<int, 1> dist_buf(dist.data(), dist.size());

    auto process_light_edges = [&](std::vector<int>& bucket) {
        sycl::buffer<int, 1> bucket_buf(bucket.data(), bucket.size());

        queue.submit([&](sycl::handler& cgh) {
            auto edge_src_acc = edge_src_buf.get_access<sycl::access::mode::read>(cgh);
            auto edge_dest_acc = edge_dest_buf.get_access<sycl::access::mode::read>(cgh);
            auto edge_weight_acc = edge_weight_buf.get_access<sycl::access::mode::read>(cgh);
            auto dist_acc = dist_buf.get_access<sycl::access::mode::read_write>(cgh);
            auto bucket_acc = bucket_buf.get_access<sycl::access::mode::read>(cgh);
            sycl::stream out(1024, 256, cgh); // stream for debugging

            cgh.parallel_for<relax_light_edges>(sycl::range<1>(bucket.size()), [=](sycl::id<1> idx) {
                int u = bucket_acc[idx];
                for (int i = 0; i < E; i++) {
                    if (edge_src_acc[i] == u && edge_weight_acc[i] <= DELTA) {
                        int v = edge_dest_acc[i];
                        int weight = edge_weight_acc[i];

                        if (dist_acc[u] != INF && dist_acc[u] + weight < dist_acc[v]) {
                            dist_acc[v] = dist_acc[u] + weight;
                            // Debug 
                            out << "Updating distance of vertex " << char('A' + v) << " to " << dist_acc[v] << " from vertex " << char('A' + u) << sycl::endl;
                        }
                    }
                }
            });
        }).wait();
    };

    auto process_heavy_edges = [&](std::vector<int>& bucket) {
        sycl::buffer<int, 1> bucket_buf(bucket.data(), bucket.size());

        queue.submit([&](sycl::handler& cgh) {
            auto edge_src_acc = edge_src_buf.get_access<sycl::access::mode::read>(cgh);
            auto edge_dest_acc = edge_dest_buf.get_access<sycl::access::mode::read>(cgh);
            auto edge_weight_acc = edge_weight_buf.get_access<sycl::access::mode::read>(cgh);
            auto dist_acc = dist_buf.get_access<sycl::access::mode::read_write>(cgh);
            auto bucket_acc = bucket_buf.get_access<sycl::access::mode::read>(cgh);
            sycl::stream out(1024, 256, cgh); // Define a stream for debugging

            cgh.parallel_for<relax_heavy_edges>(sycl::range<1>(bucket.size()), [=](sycl::id<1> idx) {
                int u = bucket_acc[idx];
                for (int i = 0; i < E; i++) {
                    if (edge_src_acc[i] == u && edge_weight_acc[i] > DELTA) {
                        int v = edge_dest_acc[i];
                        int weight = edge_weight_acc[i];

                        if (dist_acc[u] != INF && dist_acc[u] + weight < dist_acc[v]) {
                            dist_acc[v] = dist_acc[u] + weight;
                            // Debug statement 
                            out << "Updating distance of vertex " << char('A' + v) << " to " << dist_acc[v] << " from vertex " << char('A' + u) << sycl::endl;
                        }
                    }
                }
            });
        }).wait();
    };

    std::cout << "Initial distances:\n";
    print_distances(dist);

    for (int i = 0; i < buckets.size(); ++i) {
        if (buckets[i].empty()) continue; // Skip empty buckets
        std::cout << "\nProcessing bucket " << i << ":\n";
        print_buckets(buckets);

        while (!buckets[i].empty()) {
            std::vector<int> bucket = std::move(buckets[i]);
            process_light_edges(bucket);

            std::cout << "\nDistances after processing light edges of bucket " << i << ":\n";
            queue.submit([&](sycl::handler& cgh) {
                auto dist_acc = dist_buf.get_access<sycl::access::mode::read>(cgh);
                cgh.host_task([=]() {
                    for (int j = 0; j < V; ++j) {
                        std::cout << "Vertex " << char('A' + j) << " distance: " << dist_acc[j] << "\n";
                    }
                });
            }).wait();

            for (int u : bucket) {
                for (int j = 0; j < E; j++) {
                    if (edge_src[j] == u) {
                        int v = edge_dest[j];
                        int weight = edge_weight[j];

                        if (dist[v] < INF) {
                            int new_bucket_idx = dist[v] / DELTA;
                            if (new_bucket_idx > i) { // Ensure vertices are added to later buckets only
                                if (new_bucket_idx >= buckets.size()) {
                                    buckets.resize(new_bucket_idx + 1);
                                }
                                if (std::find(buckets[new_bucket_idx].begin(), buckets[new_bucket_idx].end(), v) == buckets[new_bucket_idx].end()) {
                                    buckets[new_bucket_idx].push_back(v);
                                    std::cout << "Adding vertex " << char('A' + v) << " to bucket " << new_bucket_idx << "\n";
                                }
                            }
                        }
                    }
                }
            }
            process_heavy_edges(bucket);

            std::cout << "\nDistances after processing heavy edges of bucket " << i << ":\n";
            queue.submit([&](sycl::handler& cgh) {
                auto dist_acc = dist_buf.get_access<sycl::access::mode::read>(cgh);
                cgh.host_task([=]() {
                    for (int j = 0; j < V; ++j) {
                        std::cout << "Vertex " << char('A' + j) << " distance: " << dist_acc[j] << "\n";
                    }
                });
            }).wait();
        }
    }

    std::cout << "\nFinal distances:\n";
    print_distances(dist);
}


std::vector<Edge> generateWikiGraph() {
    std::vector<Edge> edges = {
        {0, 1, 3},  // A - B
        {0, 3, 5},  // A - D
        {0, 6, 3},  // A - G
        {0, 4, 3},  // A - E
        {1, 2, 3},  // B - C
        {2, 3, 1},  // C - D
        {4, 5, 5},  // E - F
    };
    return edges;
}

std::vector<Edge> generateRandomGraph(const int num_vertices, const int num_edges) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> vertex_dis(0, num_vertices - 1);
    std::uniform_int_distribution<int> weight_dis(1, 10);

    std::vector<Edge> edges(num_edges);
    for (std::size_t i = 0; i < num_edges; ++i) {
        int src = vertex_dis(gen);
        int dest = vertex_dis(gen);
        while (dest == src) {  // no self-loops 
            dest = vertex_dis(gen);
        }
        int weight = weight_dis(gen);
        edges[i] = {src, dest, weight};
    }
    return edges;
}

std::vector<Edge> readGraphFromFile(std::string filename);

int main() {
    ////////////////////////////////////////////////////
    constexpr int V = 7;  // Number of vertices 
    constexpr int E = 7; // Number of edges
    constexpr int DELTA = 3; // Bucket size
    ////////////////////////////////////////////////////    


    // // Generate a Sparse graph
    // std::vector<Edge> edges = generateRandomGraph(50, 60);
    // print_graph(edges);

    // Generate Wiki graph
    std::vector<Edge> edges = generateWikiGraph();
    // print_graph(edges);
    deltaStepping(edges, 0, V, E, DELTA); // Source is A (vertex 0)

    return 0;       
}
