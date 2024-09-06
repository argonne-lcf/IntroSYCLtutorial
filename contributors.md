# Code contributors
* Jorge Velez <jorgev2::AT::illinois.edu> Introduction 
* George K. Thiruvathukal <gthiruvathukal::AT::luc.edu>
* Erik Pautsch <epautsch::AT::luc.edu> Introduction and Marching Cubes
* Silvio Rizzi <srizzi::AT::anl.gov> Marching Cubes
* Raymundo Hernandez-Esparza <rhernandezesparza::AT::anl.gov> Electron Density Code

* Álvaro Vázquez-Mayagoitia <alvaro::AT::anl.gov> Introduction, Examples, Electron Density

# New contributions

## Jupyter notebooks

If you are contributing any code in IPython/Jupyter notebooks, *please*
install the `nbstripout` extension (available e.g. on
[github](https://github.com/kynan/nbstripout#installation) and
[PyPI](https://pypi.org/project/nbstripout/) as `pip install nbstripout`.  After installing,
activate it for this project by running:

```
python -m nbstripout --install --attributes .gitattributes
````

from the top-level repository directory.  Please note that that
``nbstripout`` will not strip output from cells with the metadata fields
``keep_output`` or ``init_cell`` set to ``True``, so use these fields
judiciously.  You can ignore these settings with the following command:

```
   git config filter.nbstripout.extrakeys 'cell.metadata.keep_output cell.metadata.init_cell'
```
(The keys ``metadata.kernel_spec.name`` and
``metadata.kernel_spec.display_name`` may also be useful to reduce diff
noise.)

Nonetheless, it is highly discouraged to contribute code in the form of
notebooks; even with filters like ``nbstripout`` they're a hassle to use
in version control. Use them only for tutorials or *stable* examples that
are either meant to be run *interactively*.
