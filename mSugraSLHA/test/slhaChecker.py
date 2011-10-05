#! /usr/local/bin/python

import sys, os, re, math


if __name__ == '__main__':
	
	slhaFileName = sys.argv[1]
	slhaFile = open(slhaFileName, 'r')

	decayLines = []
	massBlockLines = []
	inMassBlock = False

	massFlag = True
	widthFlag = True


	for line in slhaFile:
	
		if line[:5] == 'DECAY':
			decayLines.append(line)

		if inMassBlock:
			if line[:1] == '#':
				inMassBlock = False
				continue
			massBlockLines.append(line)

		if line[:25] == '# PDG code           mass':
			inMassBlock = True

	for line in massBlockLines:
		numbers = re.findall('[0-9.\-+E]{1,16}', line)

		if float(numbers[1]) < 0 and float(numbers[0]) != 1000025:
			massFlag = False
			break

	for line in decayLines: 
		numbers = re.findall('[0-9.\-+E]{1,16}', line)

		if (float(numbers[2]) == 0.0) and (int(numbers[1]) != 1000022):
			widthFlag = False

	print slhaFileName + " " + str(widthFlag and massFlag)
