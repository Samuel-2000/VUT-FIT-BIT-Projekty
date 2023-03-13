# author: Samuel Kuchta (xkucht11), Troitckii Georgii (xtroit00)
# project: IVS projekt 2

MAIN=gui.py
MAIN_TEST=tests_final.py
MAT_LIB=mat_lib.py
MAT_TEST=test_mat_lib.py
PROFILE=profiling.py
HELP=help_text.py
STYLE=styles.py
NAMES=xkucht11_xaugus14_xhrabo17_xtroit00
PROF_INPUT=1000_int
OLD_ROOT=√
NEW_ROOT=sqroot
OLD_SUM=Σ
NEW_SUM=sum
OLD_MEAN=x̄
NEW_MEAN=x


.PHONY:
	all
	run
	test
	profile
	pack
	doc
	setup
	install
	clean

# přeloží projekt - včetně programu pro profiling (python nic nepreklada)
all:
	@echo "Python je interpretovaný jazyk, takže sa neprekladá popredu"
	@echo "Zadajte:"
	@echo "  'make run' pre spustenie kalkulacky"
	@echo "  'make test' pre spustenie testov matematickej kniznice"
	@echo "  'make test_final' pre spustenie testov GUI"
	@echo "  'make profile' pre spustenie profilingu pomocou implementacie smerodatnej odchylky nad mat_lib.py"
	@echo "  'make pack' pre zabalenie projektu do zipu pre jeho odovzdanie"
	@echo "  'make setup' pre instalaciu prerekvizit"
	@echo "  'make install' pre instalaciu kalkulacky"
	@echo "  'make clean' pre vymazanie vygenerovanych suborov"

# spustí program
run:
	python3 $(MAIN)

# spustí testy matematické knihovny
test: $(MAT_TEST)
	python3 $(MAT_TEST)

# spustí testy gui
test_final: $(MAIN_TEST)
	python3 $(MAIN_TEST)

# spustí překlad programu pro výpočet směrodatné odchylky na profiling
profile:
	python3 $(PROFILE) < ../profiling/$(PROF_INPUT)

# zabalí projekt tak, aby mohl být odevzdán
pack: clean doc 
	mkdir -p ../../$(NAMES)/doc/ && cp -a ../generated_docs/. ../../$(NAMES)/doc/
	mkdir -p ../../$(NAMES)/install/ && cp -a ../installer/. ../../$(NAMES)/install/
	mkdir -p ../../$(NAMES)/repo/ && cp -a ../. ../../$(NAMES)/repo/
	cd ../.. && zip -r $(NAMES).zip $(NAMES)

# TODO spustí generování dokumentace
doc:
#	mkdir ../doc
## copy backup
#	mkdir ../bckp
#	cp -r ../src ../bckp
##replace non-compatible characters
#	find . -name '*.py' -exec sed -i 's/$(OLD_ROOT)/$(NEW_ROOT)/g' {} \;
#	find . -name '*.py' -exec sed -i 's/$(OLD_SUM)/$(NEW_SUM)/g' {} \;
#	find . -name '*.py' -exec sed -i 's/$(OLD_MEAN)/$(NEW_MEAN)/g' {} \;
## doxypypy
#	doxypypy -a -c $(MAIN) > $(MAIN)
#	doxypypy -a -c $(MAIN_TEST) > $(MAIN_TEST)
#	doxypypy -a -c $(MAT_LIB) > $(MAT_LIB)
#	doxypypy -a -c $(MAT_TEST) > $(MAT_TEST)
#	doxypypy -a -c $(PROFILE) > $(PROFILE)
#	doxypypy -a -c $(HELP) > $(HELP)
#	doxypypy -a -c $(STYLE) > $(STYLE)
##doxygen
#	doxygen
#	mv html ../doc
#	mv latex ../doc
##recover original src files from backup
#	cp -fr ../bckp ../src

# nainstaluje prerekvizity pre instalaciu
setup:
	sudo apt-get install python3-pyqt5
#	sudo apt install python3-doxypypy

# spusti instalační skript
install:
	sudo apt-get install dh-make
	chmod +x install.sh && bash install.sh

# smaže všechny generovatelné soubory
clean:
	rm -rf __pycache__ .pytest_cache ../doc doxygen_warn_logfile.log ../../$(NAMES).zip ../bckp
