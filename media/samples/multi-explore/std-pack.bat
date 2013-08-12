@echo off
cd ironhand-package
rename package.xml package.xml.not-yet
cd ..
cd std-package
rename package.xml.not-yet package.xml