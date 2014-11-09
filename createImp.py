import sublime, sublime_plugin
import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), ".\\CppHeaderParser-2.4.3"))
sys.path.append(os.path.join(os.path.dirname(__file__), ".\\ply-3.4\\"))
import CppHeaderParser


#window.run_command("create_imp", {"header_name" : "insert file header here."})

class CreateImpCommand(sublime_plugin.WindowCommand):
	def getData(self, fileName, className):
		try:
			cppHeader = CppHeaderParser.CppHeader(fileName)
		except CppHeaderParser.CppParseError as e:
			print(e)
		testClass = cppHeader.classes[className]
		fcnList = []
		for fcnType in testClass["methods"]:
			for meth in testClass["methods"][fcnType]:
				proto = ""
				if meth["static"]:
					proto = proto + "static "
				if meth["const"]:
					proto = proto + "const "
				if meth["inline"]:
					proto = proto + "inline "
				if not meth["constructor"] and not meth["destructor"]:
					proto = proto + meth["rtnType"] + " "
				if meth["destructor"]:
					proto = proto + className + "::~" + meth["name"] + "("
				else:
					proto = proto + className + "::" + meth["name"] + "("
				count = 0;
				for param in meth["parameters"]:
					if count != 0:
						proto = proto + ", "
					if param["constant"]:
						proto = proto + "const "
					if param["static"]:
						proto = proto + "static "
					proto = proto + param["type"] + " "
					if param["reference"]:
						proto = proto + "&"
					if param["pointer"]:
						proto = proto + "*"
					proto = proto + param["name"]
					count = count + 1
				proto = proto + "){\n\t\n}"
				fcnList.append(proto)
				#print proto
		return fcnList
			
	def insertFcns(self, view, header_name, fcnList):
		edit = view.begin_edit();
		apos = view.sel()[0].begin()
		view.insert (edit, apos, 'import "' + header_name + '"')
		for fcn in fcnList:
			pos = view.sel()[0].begin()
			view.insert(edit, pos, "\n\n" + fcn)
		view.end_edit(edit)
		
	def run(self, header_name):
		view = self.window.new_file()
		self.window = sublime.active_window()
		self.insertFcns(view, header_name, self.getData(header_name, "Hi"))