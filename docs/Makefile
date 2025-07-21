.PHONY: build autobuild clean

# This installs all the dependencies and rebuilds the entire thing.
# This is supposed to reliably work at all times.
build:
	@test -n "$(VIRTUAL_ENV)" \
		|| ( \
			echo "Error: you need to set up a Python virtual environment:" \
			&& echo "  $$ python3 -m venv .venv" \
			&& echo "  $$ source .venv/bin/activate    # add .fish/.csh/.ps1 for other shells" \
			&& exit 1 \
		)

	pip install --quiet sphinx furo
	sphinx-build . build --quiet --write-all

autobuild: build
	pip install --quiet sphinx-autobuild
	sphinx-autobuild . build --quiet --no-initial --open-browser

clean:
	rm -rf build

