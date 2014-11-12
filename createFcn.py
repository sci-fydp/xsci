import sublime, sublime_plugin


#window.run_command("create_imp", {"header_name" : "insert file header here."})

class CreateFcnCommand(sublime_plugin.TextCommand):
	def parseData(self, data):
		proto = ""
		if data["static"]:
			proto = proto + "static "
		if data["const"]:
			proto = proto + "const "
		if data["inline"]:
			proto = proto + "inline "
		if data["virtual"]:
			proto = proto + "virtual "
		if data["friend"]:
			proto = proto + "friend "

		if not data["constructor"] and not data["destructor"]:
			proto = proto + data["rtnType"] + " "

		if data["destructor"]:
			proto = proto + "~" + data["className"] + "("
		elif data["constructor"]:
			proto = proto + data["className"] + "("
		else:
			proto = proto + data["name"] + "("

		count = 0;
		for param in data["parameters"]:
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
		proto = proto + ")"
		if data["pureVirtual"]:
			proto = proto + " = 0;"
		elif data["proto"]:
			proto = proto + ";"
		else:
			proto = proto + "{\n\t\n}"
		#print proto
		return proto
			
	def insertFcns(self, edit, fcnStr, extra):
		v = self.view
		pos = v.sel()[0].begin()
		v.insert(edit, pos, fcnStr)
		if not extra:
			pos = v.sel()[0].begin()
			v.sel().clear()
			v.sel().add(sublime.Region(pos - 2,pos - 2))
		
	def run(self, edit, args):
		self.insertFcns(edit, self.parseData(args), args["proto"] | args["pureVirtual"])
