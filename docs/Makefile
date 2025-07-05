.PHONY: all setup build open-preview clean

all: setup build

setup: .setup-done

# To install the dependencies you could also you could try:
# $ python3 -m pip install sphinx breathe furo
.setup-done:
	python3 -m pip install -r requirements.txt \
		|| python3 -m venv --help \
		&& touch .setup-done

build:
	doxygen -q
	sphinx-build -q . ../build/docs/html || python3 -m venv --help

open-preview:
	open ../build/docs/html/index.html

clean:
	rm -rf ../build/docs
