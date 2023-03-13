#!/bin/bash

PROGRAM_NAME='calculator-1.0'
DEV_EMAIL="xtroit00@stud.fit.vutbr.cz"
LICENCE="gpl2"

cp gui.py ../installer/"$PROGRAM_NAME"/gui.py && chmod +x ../installer/"$PROGRAM_NAME"/gui.py
cp mat_lib.py ../installer/"$PROGRAM_NAME"/mat_lib.py && chmod +x ../installer/"$PROGRAM_NAME"/mat_lib.py
cp styles.py ../installer/"$PROGRAM_NAME"/styles.py && chmod +x ../installer/"$PROGRAM_NAME"/styles.py
cp help_text.py ../installer/"$PROGRAM_NAME"/help_text.py && chmod +x ../installer/"$PROGRAM_NAME"/help_text.py
sudo cp ../installer/"$PROGRAM_NAME"/install/calc.xpm /usr/share/pixmaps

cd ../installer
tar -czf "$PROGRAM_NAME".tar.gz "$PROGRAM_NAME"

cd "$PROGRAM_NAME"
dh_make -e "$DEV_EMAIL" -n -s -c "$LICENCE" -f ../"$PROGRAM_NAME".tar.gz

cp ../control debian/control
cp ../install debian/install

sudo dpkg-buildpackage -rfakeroot -uc -b

sudo rm -r debian
rm gui.py mat_lib.py styles.py help_text.py
cd ..
rm calculator-1.0.tar.gz
sudo rm calculator_1.0_amd64.changes calculator_1.0_amd64.buildinfo




