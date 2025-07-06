.PHONY: all setup build check-venv open-preview clean

all: setup build

check-venv:
	@test -n "$(VIRTUAL_ENV)" \
		|| ( \
			echo "You need to set up a Python virtual environment:" \
			&& echo "$$ python3 -m venv .venv" \
			&& echo "$$ source .venv/bin/activate    # add .fish/.csh/.ps1 for other shells" \
			&& exit 1 \
		)

setup: check-venv .setup-done

# To install the dependencies you could also you could try:
# $ python3 -m pip install sphinx breathe furo
.setup-done:
	python3 -m pip install -r requirements.txt && touch .setup-done

build: check-venv
	doxygen -q
	sphinx-build -q . ../build/docs/html

open-preview:
	open ../build/docs/html/index.html

clean:
	rm -rf ../build/docs
