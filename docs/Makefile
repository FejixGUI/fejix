.PHONY: build dev clean

# This installs all the dependencies and rebuilds the entire thing.
build:
	python3 -m pip install sphinx furo hawkmoth --quiet
	sphinx-build ./ ./build/ --write-all --quiet

# This runs a local server that incrementally rebuilds the documentation
# automatically when files change.
dev: build
	python3 -m pip install sphinx-autobuild --quiet
	sphinx-autobuild ./ ./build/ --watch ../include --no-initial --open-browser --quiet

clean:
	rm -rf ./build/

