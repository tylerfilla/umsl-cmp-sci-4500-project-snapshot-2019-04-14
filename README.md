# app

*For the time being, use the following steps or equivalent to develop.*

Clone the repo.
```sh
$ git clone https://github.com/cozmonauts/app.git
$ cd app
```

Initialize submodules in the folder. This is not strictly necessary until you
see a ```third_party/``` folder appear in the repo, but it's a good habit.
```sh
$ git submodule update --init --recursive
```

Run the client module. This executes the ```__main__.py``` file in
```python/cozmonaut/```, and it also sets up the module path in the VM to scan
```python/``` for modules.
```sh
$ cd python
$ python -m cozmonaut
```

*Within the week, we will have a system in place to launch the Python code
from a simple executable that also supplies face recognition services. The
customer will only need to run one executable. __Until then, this will let us
work on other functionality.__*
