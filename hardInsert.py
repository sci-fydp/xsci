import sublime, sublime_plugin

class HardInsertCommand(sublime_plugin.TextCommand):
	def run(self, edit, text):
		pos = self.view.sel()[0].begin()
		self.view.insert(edit, pos, text)
