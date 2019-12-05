import os

os.system('JLink -device stm32f103c8 -if swd -speed 100 -autoconnect 1 -CommanderScript .\JLinkCommandFile.jlink')
