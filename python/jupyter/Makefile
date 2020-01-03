#-------------------------------
## help                         : Display this help.
.PHONY: help
help: Makefile
	@sed -n 's/^##//p' $<


##
#-------------------------------
## venv-activate                : Run this first, which creates and activates virtualenv.
.PHONY: venv-activate
venv-activate:
	python3 -m venv .
	@echo Please run "'source bin/activate'" in your shell.
	@echo More information about virtualenv can be found at https://docs.python.org/3/tutorial/venv.html


#-------------------------------
## init-again                   : Install all python requirements.
.PHONY: init-again
init-again:
	pip install -r requirements.txt


#-------------------------------
## init                         : Initialize environment for the first time.
.PHONY: init
init:
	@echo More information about jupyter can be found at https://jupyter.org/install
	pip install jupyterlab

	pip install seaborn


#-------------------------------
## run-jupyter                  : Run jupyter.
.PHONY: run-jupyter
run-jupyter:
	@echo https://jupyter.readthedocs.io/en/latest/running.html
	jupyter notebook
# 	jupyter notebook --port 9999
# 	jupyter notebook --no-browser
