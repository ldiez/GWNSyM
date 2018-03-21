import xml.etree.ElementTree as ET
from subprocess import call

# Open original file

execu = './scenarios/debug/devScenario'
cnfFile = './config-files/DEV/USER.xml'
vals = ['0.4', '0.5', '0.6', '0.7', '0.8', '0.9', '1.0']	

for v in vals:
	tree = ET.parse(cnfFile)
	root = tree.getroot()
	thingy = root.find('LTE_UE/UL_ALPHA')
	thingy.text = v
	tree.write(cnfFile)
	call([execu])
	