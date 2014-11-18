import sublime, sublime_plugin

#window.run_command("new_header", {"header_name" : "hi"})
        
class CreateFileCommand(sublime_plugin.WindowCommand):

    def run(self, header_name):
        view = self.window.new_file()
        self.window = sublime.active_window()



