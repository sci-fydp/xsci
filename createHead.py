import sublime, sublime_plugin

#window.run_command("new_header", {"header_name" : "hi"})
        
class NewHeaderCommand(sublime_plugin.WindowCommand):

    def insertTemplate(self, view, template):
        edit = view.begin_edit();
        str = "\nclass " + template + " {\n\t\n}"
        #index = str.find("\t")
        view.insert(edit, 0, str)
        #view.show_at_center(index)
        #self.gotoLine(view, edit, 3)
        view.end_edit(edit)

    def run(self, header_name):
        view = self.window.new_file()
        self.window = sublime.active_window()
        self.insertTemplate(view, header_name)



