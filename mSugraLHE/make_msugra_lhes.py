import os,sys,glob, re, shutil
from xml.dom import minidom

def mySort(aa,bb):
	aa = aa.replace('.slha', '')
	bb = bb.replace('.slha', '')
	a=aa.split("/")[-1]
	b=bb.split("/")[-1]
	sa=a.split("_")
	sb=b.split("_")

	m0a=float(sa[1])
	m0b=float(sb[1])

	m12a=float(sa[2])
	m12b=float(sb[2])

	if m0a!=m0b: return cmp(m0a,m0b)
	return cmp(m12a,m12b)

def makeLhe(configFile, slhaFileDir, slhaFile, nEvents, randomNumberSeed):
	slhaFileDirCommand = ('sed -i -e "s,SLHADIR,'+ str(slhaFileDir)+ '," ' +
						  str(configFile))
	slhaFileNameCommand = ('sed -i -e "s,RUNSLHA,' + str(slhaFile) + '," ' +
						   str(configFile))
	nEventsCommand = ('sed -i -e "s,NUMEVTS,' + str(nEvents) + '," ' +
					  str(configFile))
	randomNumberCommand = ('sed -i -e "s,MYRANSEED,' + str(randomNumberSeed) +
						   '," ' + str(configFile))

	os.system(slhaFileDirCommand)
	os.system(slhaFileNameCommand)
	os.system(nEventsCommand)
	os.system(randomNumberCommand)
	os.system('cmsRun ' + str(configFile) + " > logFile.txt")
def insertModelLine(lheFile, modelName, logFileName):

	logFile = open(logFileName, 'r')
	for line in logFile:
		if line.find('All included sub') > -1:
			numberStrings = line.split("I")
			numberStrings[3] = numberStrings[3].replace('D','E')
			xsection = str(float(numberStrings[3]))
		if line.find('Fraction of') > -1:
			numberStrings = line.split("=")
			numberStrings = numberStrings[1].split()
			efficiency =str(1.0 - float(numberStrings[0]))

	
	try:
		xmldoc = minidom.parse(lheFile)
	except IOError:
		print " could not open file for xml parsing " + lheFile
		sys.exit(0)

	shutil.copyfile(lheFile,lheFile+".bak")
	reflist = xmldoc.firstChild

	for ref in reflist.childNodes:
		if ref.nodeName=='header':
			ref.appendChild(x)
		if ref.nodeName=='event':
			ref.firstChild.appendData("# model " + modelName + " " + xsection +
									  " " + efficiency + "\n")

	t=xmldoc.toprettyxml(indent="",newl="")
	f=open(lheFile,'w')
	f.write(t)
	f.close()
	logFile.close()
	f=open(lheFile,'r')
	o=open(lheFile+".tmp",'w')

	for i, line in enumerate(f):
		if line.find("-->")>-1:
			o.write(line)
			o.write('<header>\n')
			o.write('<slha>\n')
			o.write("DECAY   1000022   0.0E+00\n")
			o.write('</slha>\n')				
			o.write('</header>\n')
		else:
			o.write(line)

	f.close()
	o.close()
	os.remove(lheFile)
	os.rename(lheFile+".tmp", lheFile)


def mergeEvents(inFileList, outFileName):

	startWritingString = "<event"
	endWritingString = "</LesHouchesEvents>"
	isWriting = False
	outFile = open(outFileName, 'a')

	for fileName in inFileList:

		file = open(fileName, 'r')

		for line in file:
			if isWriting == True and len(line) > 0:
				if line.find(endWritingString) > -1:

					isWriting = False
					break
				
				outFile.write(line)
			else:
				if line.find(startWritingString) > -1:

					isWriting = True
					outFile.write(line)
		file.close()
	outFile.close()
		
def fromFileToFile(beginFileName, endFileName):
	combinedFileName = ''
	preFix = beginFileName.split('_')[0]
	postFix = beginFileName.split('.')[-1]
	tanb = beginFileName.split('_')[3]
	A0 = beginFileName.split('_')[4]
	sign = beginFileName.split('_')[5]
	sign = sign.split('.')[0]
	
	beginM0 = beginFileName.split('_')[1]
	beginM12 = beginFileName.split('_')[2]
	endM0 = endFileName.split('_')[1]
	endM12 = endFileName.split('_')[2]

	combinedFileName = preFix + '_' + beginM0 + '_' + beginM12 + 'To' + endM0 + '_' + endM12 +  '_' + tanb + '_' + A0 + '_' + sign + '.' + postFix
	return combinedFileName
def startOutputFile(fileName):
	header=str("<LesHouchesEvents>\n\
<!--\n\
File generated with PYTHIA 6.424\n\
-->\n\
<header>\n\
<MGVersion>\n\
# MG/ME version    : 4.0.0\n\
</MGVersion>\n\
<MGProcCard>\n\
# Begin PROCESS # This is TAG. Do not modify this line\n\
pp>MODELTAG    @0       # First Process\n\
# End PROCESS  # This is TAG. Do not modify this line\n\
</MGProcCard>\n\
<MGGenerationInfo>\n\
#  Number of Events        :      NUMEVT\n\
</MGGenerationInfo>\n\
<slha>\n\
DECAY   1000022   0.0E+00\n\
</slha>\n\
</header>\n\
<init>\n\
    2212    2212  3.500000E+03  3.500000E+03     0     0 10042 10042     3    84\n\
  1.000000E+00  1.000000E+00  1.000000E+00   201\n\
  1.000000E+00  1.000000E+00  1.000000E+00   202\n\
  1.000000E+00  1.000000E+00  1.000000E+00   204\n\
  1.000000E+00  1.000000E+00  1.000000E+00   205\n\
  1.000000E+00  1.000000E+00  1.000000E+00   207\n\
  1.000000E+00  1.000000E+00  1.000000E+00   208\n\
  1.000000E+00  1.000000E+00  1.000000E+00   209\n\
  1.000000E+00  1.000000E+00  1.000000E+00   210\n\
  1.000000E+00  1.000000E+00  1.000000E+00   211\n\
  1.000000E+00  1.000000E+00  1.000000E+00   212\n\
  1.000000E+00  1.000000E+00  1.000000E+00   213\n\
  1.000000E+00  1.000000E+00  1.000000E+00   214\n\
  1.000000E+00  1.000000E+00  1.000000E+00   216\n\
  1.000000E+00  1.000000E+00  1.000000E+00   217\n\
  1.000000E+00  1.000000E+00  1.000000E+00   218\n\
  1.000000E+00  1.000000E+00  1.000000E+00   219\n\
  1.000000E+00  1.000000E+00  1.000000E+00   220\n\
  1.000000E+00  1.000000E+00  1.000000E+00   221\n\
  1.000000E+00  1.000000E+00  1.000000E+00   222\n\
  1.000000E+00  1.000000E+00  1.000000E+00   223\n\
  1.000000E+00  1.000000E+00  1.000000E+00   224\n\
  1.000000E+00  1.000000E+00  1.000000E+00   225\n\
  1.000000E+00  1.000000E+00  1.000000E+00   226\n\
  1.000000E+00  1.000000E+00  1.000000E+00   227\n\
  1.000000E+00  1.000000E+00  1.000000E+00   228\n\
  1.000000E+00  1.000000E+00  1.000000E+00   229\n\
  1.000000E+00  1.000000E+00  1.000000E+00   230\n\
  1.000000E+00  1.000000E+00  1.000000E+00   231\n\
  1.000000E+00  1.000000E+00  1.000000E+00   232\n\
  1.000000E+00  1.000000E+00  1.000000E+00   233\n\
  1.000000E+00  1.000000E+00  1.000000E+00   234\n\
  1.000000E+00  1.000000E+00  1.000000E+00   235\n\
  1.000000E+00  1.000000E+00  1.000000E+00   236\n\
  1.000000E+00  1.000000E+00  1.000000E+00   237\n\
  1.000000E+00  1.000000E+00  1.000000E+00   238\n\
  1.000000E+00  1.000000E+00  1.000000E+00   239\n\
  1.000000E+00  1.000000E+00  1.000000E+00   240\n\
  1.000000E+00  1.000000E+00  1.000000E+00   241\n\
  1.000000E+00  1.000000E+00  1.000000E+00   242\n\
  1.000000E+00  1.000000E+00  1.000000E+00   243\n\
  1.000000E+00  1.000000E+00  1.000000E+00   244\n\
  1.000000E+00  1.000000E+00  1.000000E+00   246\n\
  1.000000E+00  1.000000E+00  1.000000E+00   247\n\
  1.000000E+00  1.000000E+00  1.000000E+00   248\n\
  1.000000E+00  1.000000E+00  1.000000E+00   249\n\
  1.000000E+00  1.000000E+00  1.000000E+00   250\n\
  1.000000E+00  1.000000E+00  1.000000E+00   251\n\
  1.000000E+00  1.000000E+00  1.000000E+00   252\n\
  1.000000E+00  1.000000E+00  1.000000E+00   253\n\
  1.000000E+00  1.000000E+00  1.000000E+00   254\n\
  1.000000E+00  1.000000E+00  1.000000E+00   256\n\
  1.000000E+00  1.000000E+00  1.000000E+00   258\n\
  1.000000E+00  1.000000E+00  1.000000E+00   259\n\
  1.000000E+00  1.000000E+00  1.000000E+00   261\n\
  1.000000E+00  1.000000E+00  1.000000E+00   262\n\
  1.000000E+00  1.000000E+00  1.000000E+00   263\n\
  1.000000E+00  1.000000E+00  1.000000E+00   264\n\
  1.000000E+00  1.000000E+00  1.000000E+00   265\n\
  1.000000E+00  1.000000E+00  1.000000E+00   271\n\
  1.000000E+00  1.000000E+00  1.000000E+00   272\n\
  1.000000E+00  1.000000E+00  1.000000E+00   273\n\
  1.000000E+00  1.000000E+00  1.000000E+00   274\n\
  1.000000E+00  1.000000E+00  1.000000E+00   275\n\
  1.000000E+00  1.000000E+00  1.000000E+00   276\n\
  1.000000E+00  1.000000E+00  1.000000E+00   277\n\
  1.000000E+00  1.000000E+00  1.000000E+00   278\n\
  1.000000E+00  1.000000E+00  1.000000E+00   279\n\
  1.000000E+00  1.000000E+00  1.000000E+00   280\n\
  1.000000E+00  1.000000E+00  1.000000E+00   281\n\
  1.000000E+00  1.000000E+00  1.000000E+00   282\n\
  1.000000E+00  1.000000E+00  1.000000E+00   283\n\
  1.000000E+00  1.000000E+00  1.000000E+00   284\n\
  1.000000E+00  1.000000E+00  1.000000E+00   285\n\
  1.000000E+00  1.000000E+00  1.000000E+00   286\n\
  1.000000E+00  1.000000E+00  1.000000E+00   287\n\
  1.000000E+00  1.000000E+00  1.000000E+00   288\n\
  1.000000E+00  1.000000E+00  1.000000E+00   289\n\
  1.000000E+00  1.000000E+00  1.000000E+00   290\n\
  1.000000E+00  1.000000E+00  1.000000E+00   291\n\
  1.000000E+00  1.000000E+00  1.000000E+00   292\n\
  1.000000E+00  1.000000E+00  1.000000E+00   293\n\
  1.000000E+00  1.000000E+00  1.000000E+00   294\n\
  1.000000E+00  1.000000E+00  1.000000E+00   295\n\
  1.000000E+00  1.000000E+00  1.000000E+00   296\n\
</init>\n")
	header = header.replace("MODELTAG", fileName.split('_')[0])
	file = open(fileName, 'w')
	file.writelines(header)
	file.close()
def endOutputFile(fileName):
	file = open(fileName, 'r')
	numberOfEvents = 0
	
	for line in file:
		if line.find("<event") > -1:
			numberOfEvents = numberOfEvents + 1

	file.close()
	file = open(fileName , 'a')

	file.writelines("</LesHouchesEvents>")
	file.close()

	os.system('sed -i 1,20s/NUMEVT/' + str(numberOfEvents) + '/ ' + fileName)
def checkLhe(lheFileName):
	file = open(lheFileName, 'r')

	for line in file:
		if line.find('1000022') == 1:
			split = line.split(" ")
			for char in split[2:]:
				if char == '': continue
				if char == '1': break
				print "Unstable Chi_10"
				return False
		elif line.find('0000') == 2:
			split = line.split(" ")
			for char in line.split(" "):
				if char =='': continue
				if char != '1': break
				print "Stable SUSY particle that isn't Chi_10"
				return False
	return True


#################################	
############  MAIN  #############
#################################	
if __name__ == '__main__':
	if len(sys.argv) < 3:
		print "Usage <number of events per point> <slha file directory> <slha file list>"
		sys.exit(0)
	randomNumberSeed = 0
	eventsPerPoint = int(sys.argv[1])
	slhaDir = sys.argv[2]
#	slhaDir = 'UserCode/Summer11LHERuns/mSugraSLHA/test'
	slhaFileList = sys.argv[3:]
	slhaFileList.sort(mySort)

	firstFileName = slhaFileList[0].split('/')[-1]
	lastFileName = slhaFileList[-1].split('/')[-1]

	if firstFileName != lastFileName:
		outputFileName = fromFileToFile(firstFileName.replace('.slha','.lhe'),
										lastFileName.replace('.slha', '.lhe'))
	else:
		outputFileName = firstFileName.replace('.slha', '.lhe')

	startOutputFile(outputFileName)

	for slhaFile in slhaFileList:

		slhaFile = slhaFile.split('/')[-1]
		randomNumberSeed = randomNumberSeed + 1

		modelTag = slhaFile.split('/')[-1]
		modelTag = modelTag.replace('.slha', '')

		os.system("cp slhaTolhe_msugra_cfg.py " +
				  "slhaTolhe_temp_msugra_cfg.py")
		makeLhe("slhaTolhe_temp_msugra_cfg.py",
				slhaDir, slhaFile.split('/')[-1],
				eventsPerPoint, randomNumberSeed)
		if glob.glob('fort.69') == []:
			continue
		insertModelLine('fort.69', modelTag, 'logFile.txt')
		if checkLhe('fort.69') == False :
			os.system("rm fort.69")
			os.system("rm slhaTolhe_temp_msugra_cfg.py")
			print modelTag + '.lhe - Bad point, ommiting from file'
			continue
		mergeEvents(['fort.69'], outputFileName)

		os.system("rm fort.69")
		os.system("rm slhaTolhe_temp_msugra_cfg.py")
		os.system("rm logFile.txt")
		
	endOutputFile(outputFileName)

