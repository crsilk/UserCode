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

	
	
