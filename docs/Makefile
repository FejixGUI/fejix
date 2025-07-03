.PHONY: all setup build open-preview clean

all: setup build

setup: .setup-done

.setup-done:
	python3 -m pip install -r requirements.txt && touch .setup-done || (echo && echo "Note: to setup Python venv run:" && echo " $$ python3 -m venv .venv && source .venv/bin/activate  # or activate.fish, activate.csh etc." && exit 1)

build:
	doxygen -q
	sphinx-build -q . ../build/docs/html

open-preview:
	open ../build/docs/html/index.html

clean:
	rm -rf ../build/docs
