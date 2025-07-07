.PHONY: all setup build clean preview check-python-venv

all: build

setup: check-python-venv ../build/docs

build: setup ./Doxyfile ./conf.py
	doxygen -q
	sphinx-build -q . ../build/docs/html

clean:
	rm -rf ../build/docs

preview:
	open ../build/docs/html/index.html

check-python-venv:
	@test -n "$(VIRTUAL_ENV)" \
		|| ( \
			echo "Error: you need to set up a Python virtual environment:" \
			&& echo "  $$ python3 -m venv .venv" \
			&& echo "  $$ source .venv/bin/activate    # add .fish/.csh/.ps1 for other shells" \
			&& exit 1 \
		)

../build/docs: ./requirements.txt
	python3 -m pip install -r requirements.txt && mkdir -p ../build/docs

