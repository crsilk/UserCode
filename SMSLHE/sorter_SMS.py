import os,sys,glob, re, shutil
from xml.dom import minidom

def mySort(aa,bb):
	aaa = aa.replace('.slha', '')
	bbb = bb.replace('.slha', '')
	a=aaa.split("/")[-1]
	b=bbb.split("/")[-1]
	sa=a.split("_")
	sb=b.split("_")

	m0a=float(sa[2])
	m0b=float(sb[2])

	m12a=float(sa[3])
	m12b=float(sb[3])

	if m0a!=m0b: return cmp(m0a,m0b)
	return cmp(m12a,m12b)

if __name__ == "__main__":
	jobNumber = int(sys.argv[1])
	numberOfFilesPerJob = int(sys.argv[2])
	list = sys.argv[3:]

	list.sort(mySort)
	output =''

	if len(list) >= ((jobNumber+1)*numberOfFilesPerJob):
		for name in  list[(jobNumber * numberOfFilesPerJob):
						  ((jobNumber+1)*numberOfFilesPerJob)]:
			output = output + name.split('/')[-1] + ' '
		print output
			
	elif len(list) >= jobNumber * numberOfFilesPerJob:
		for name in list[(jobNumber * numberOfFilesPerJob):]:
			output = output + name.split('/')[-1] + ' '

		print output

	
	
