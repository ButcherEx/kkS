import xmllib

class VCProjParser(xmllib.XMLParser):
	def __init__(self,parent):
		xmllib.XMLParser.__init__(self)
		self.parent = parent
		self.config = ''
		self.curtag = ''

	def handle_starttag(self, tag, method, attributes):
		print tag

	def unknown_starttag(self, tag, attributes):
		tagtemp = tag.split(" ")
		tag = tagtemp[1]
		self.curtag = tag
		if tag == 'ClCompile':
			for x in attributes.items():
				if x[0] == 'Include':
					self.parent.addfile(x[1])

	def handle_data(self, data):
		if self.curtag == 'RootNamespace':
			self.parent.setprjname(data)
		elif self.curtag == 'AdditionalIncludeDirectories':
			self.parent.includes= self.incdir2list(data)
		elif self.curtag == 'PreprocessorDefinitions':
			self.parent.defines = self.def2list(data)

	def unknown_endtag(self,tag):
		self.curtag = ''

	def incdir2list(self, incdir):
		incdir = incdir.replace('\\','/').replace('%(AdditionalIncludeDirectories)',';').replace(';;', '')
		return incdir.split(';')

	def def2list(self, define):
		define = define.replace('WIN32',';').replace('__WINDOWS__',';').replace('%(PreprocessorDefinitions)',';').replace(';;', '').replace(';;', '').replace(';;', '')
		return define.split(';')

	def pathnormal(self, path):
		return path.replace('\\','/')


import fnmatch
import os
import os.path
import re

class Project:
	allowfile = ['*.c', '*.cpp', '*.cc','*.hpp', '*.h']	
	def __init__(self, style='exe'):
		self.parser = VCProjParser(self)
		self.prjname = ''
		self.files=[]
		self.includes = []
		self.defines = []
		self.style = style
		self.includes = ''
		self.basedir = ''
		self.amre = '^(lib_)?%s_(.+)\s*=(.+)$'
		self.amset = {}
		self.amset1 = {}

	def makeuniqname(self):
		if self.style == 'exe':
			return "%s" % self.prjname
		if self.style == 'lib':
			return "lib%s_a" % self.prjname

	def loadvcproj(self, vcproj):
		self.parser.reset()
		f = open(vcproj, "r")
		data = f.read()
		self.parser.feed(data[3:])
		self.parser.close()
		f.close()

		self.defines.append('__LINUX__')
		self.amset['SOURCES'] = " ".join(self.files)
		self.amset["DEFINES"] = " ".join(["-D" + l for l in self.defines])
		self.amset["INCLUDES"] = " ".join(["-I" + l for l in self.includes])

		self.amset1['CFLAGS'] = "$(%s_DEFINES) $(%s_INCLUDES)" % (self.makeuniqname(), self.makeuniqname())
		self.amset1['CXXFLAGS'] = "$(%s_DEFINES) $(%s_INCLUDES)" % (self.makeuniqname(), self.makeuniqname())
		if self.style != 'lib':
			self.amset['LDADD'] = "$(ALLLIBS)"

	def mergemakeam(self, amfile):
		mretext = self.amre % self.makeuniqname()
		myre = re.compile(mretext)

		try:
			f = open(amfile, "r")
			for l in f:
				m = myre.match(l)
				if m:
					k = m.group(2).replace(' ','')
					v = m.group(3)
					if k == 'SOURCES' : continue
					if k == 'CXXFLAGS' or k == 'CFLAGS':
						self.amset1[k] = v
					else:
						self.amset[k] = v
			f.close()
		except:
			pass

	def addfile(self, f):
		f = f.replace('\\','/')
		for a in Project.allowfile:
			if fnmatch.fnmatch(f, a):
				self.files.append(f)
				return

	def setprjname(self, n):
		self.prjname = n.split('.')[-1]

	def output(self, outf):
		for k in self.amset.keys():
			outf.write("%s_%s = %s\n" % (self.makeuniqname(), k, self.amset[k]))

		for k in self.amset1.keys():
			outf.write("%s_%s = %s\n" % (self.makeuniqname(), k, self.amset1[k]))

		outf.write("\n")

import sys
class makefile:
	def __init__(self, path):
		self.ldadd=''
		self.text=''
		self.count = 0
		self.progs =[]
		self.libs = []
		if path == '':
			self.makefile = "Makefile.am"
			self.path = '.'
		else:
			self.makefile = path + "/Makefile.am"
			self.path = path

	def findproj(self):
		for fn in os.listdir(self.path):
			if fnmatch.fnmatch(fn, '*.vcxproj'):
				p = Project()
				p.loadvcproj(self.path + '/' + fn)
				p.mergemakeam(self.makefile)
				p.output(self)
				if p.style == "lib":
					self.libs.append(p.makeuniqname().replace('_','.'))

				if p.style == "exe":
					self.progs.append(p.makeuniqname())

				self.count = self.count + 1

	def write(self, str):
		self.text = self.text + str


	def output(self):
		if self.count == 0:
			print "No *.vcxproj files found!"
			return

		outf = open(self.makefile, "w")
		outf.write('include $(top_srcdir)/Makefile.common.in\n')
		if len(self.progs) > 0:
			outf.write('bin_PROGRAMS=%s\n' % " ".join(self.progs))
		if len(self.libs) > 0:
			outf.write('lib_LIBRARIES=%s\n' % " ".join(self.libs))

		outf.write(self.text)
		outf.close()


if len(sys.argv) == 1:
	path = "."
else:
	path = sys.argv[1]

m = makefile(path)
m.findproj()
m.output()
		