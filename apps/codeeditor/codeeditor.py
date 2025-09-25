import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import subprocess
import sys
import os

RUN_DEFAULT = "workspace.py"  # fallback run target when no file is loaded

class TKinterApp:
    def __init__(self, root):
        self.root = root
        self.root.title("tKinter")
        self.root.geometry("1100x700")

        # Layout: left toolbar (fixed), right editor (expands)
        self.root.grid_columnconfigure(0, weight=0)  # toolbar
        self.root.grid_columnconfigure(1, weight=1)  # editor area
        self.root.grid_rowconfigure(0, weight=1)

        # Toolbar (left)
        self.toolbar = ttk.Frame(self.root, padding=6)
        self.toolbar.grid(row=0, column=0, sticky="ns")

        # Editor container (right)
        self.editor_frame = ttk.Frame(self.root)
        self.editor_frame.grid(row=0, column=1, sticky="nsew")
        self._build_editor()

        # Buttons
        for text, cmd in [
            ("Run",           self.run_code),
            ("Save",          self.save_code),
            ("Load",          self.open_code),
            ("Close Code",    self.close_code),
            ("Reset Editor",  self.reset_editor),
            ("Copy All",      self.copy_all),
            ("Export to TXT", self.export_txt),
            ("Exit",          self.close_tkinter),
        ]:
            ttk.Button(self.toolbar, text=text, command=cmd).pack(fill="x", pady=3)

        self.current_path = None
        self._update_title()

    # ---------- Editor construction / reset ----------
    def _build_editor(self):
        self.editor = tk.Text(self.editor_frame, wrap="none", undo=True)
        self.editor.pack(fill="both", expand=True)

        # Focus binding only; let Tkinter handle Ctrl+V natively
        self.editor.bind("<Button-1>", lambda e: self.editor.focus_set())

        # Optional: disable Ctrl+Shift+V if you dislike it
        self.editor.bind("<Control-Shift-v>", lambda e: "break")

    def reset_editor(self):
        try:
            self.editor_frame.destroy()
        except tk.TclError:
            pass
        self.editor_frame = ttk.Frame(self.root)
        self.editor_frame.grid(row=0, column=1, sticky="nsew")
        self._build_editor()
        self.current_path = None
        self._set_editor_enabled(True)
        self._update_title()

    # ---------- File operations ----------
    def save_code(self):
        if not self.current_path:
            path = filedialog.asksaveasfilename(
                defaultextension=".py",
                filetypes=[("Python Files", "*.py"), ("All Files", "*.*")]
            )
            if not path:
                return
            self.current_path = path

        try:
            with open(self.current_path, "w", encoding="utf-8") as f:
                f.write(self.editor.get("1.0", "end-1c"))
            self._update_title()
        except Exception as e:
            messagebox.showerror("Save Error", f"{type(e).__name__}: {e}")

    def _load_path(self, path):
        """Internal: load file content into editor and set current_path."""
        if not os.path.exists(path):
            create = messagebox.askokcancel("File Not Found",
                                            f"File not found:\n{path}\n\nCreate new file?")
            if not create:
                return
            try:
                with open(path, "w", encoding="utf-8") as f:
                    f.write("")
            except Exception as e:
                messagebox.showerror("Create Error", f"{type(e).__name__}: {e}")
                return

        with open(path, "r", encoding="utf-8") as f:
            content = f.read()
        self._set_editor_enabled(True)
        self.editor.delete("1.0", "end")
        self.editor.insert("1.0", content)
        self.current_path = path
        self._update_title()

    def open_code(self):
        path = filedialog.askopenfilename(
            filetypes=[("Python Files", "*.py"), ("All Files", "*.*")]
        )
        if not path:
            return
        try:
            self._load_path(path)
        except Exception as e:
            messagebox.showerror("Open Error", f"{type(e).__name__}: {e}")

    # ---------- Run ----------
    def run_code(self):
        target = self.current_path or RUN_DEFAULT
        if not os.path.isabs(target):
            target = os.path.abspath(target)

        if not os.path.exists(target):
            create = messagebox.askokcancel("Run Error",
                                            f"File not found:\n{target}\n\nCreate new file?")
            if not create:
                return
            try:
                with open(target, "w", encoding="utf-8") as f:
                    f.write("")
                self._load_path(target)
            except Exception as e:
                messagebox.showerror("Create Error", f"{type(e).__name__}: {e}")
                return

        try:
            subprocess.Popen([sys.executable, target])
        except Exception as e:
            messagebox.showerror("Run Error", f"{type(e).__name__}: {e}")

    # ---------- Close / disable editor ----------
    def close_code(self):
        self._set_editor_enabled(True)
        self.editor.delete("1.0", "end")
        self._set_editor_enabled(False)
        self.current_path = None
        self._update_title()

    # ---------- New features ----------
    def copy_all(self):
        text = self.editor.get("1.0", "end-1c")
        self.root.clipboard_clear()
        self.root.clipboard_append(text)
        self.root.update()

    def export_txt(self):
        path = filedialog.asksaveasfilename(
            defaultextension=".txt",
            filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")]
        )
        if not path:
            return
        try:
            with open(path, "w", encoding="utf-8") as f:
                f.write(self.editor.get("1.0", "end-1c"))
        except Exception as e:
            messagebox.showerror("Export Error", f"{type(e).__name__}: {e}")

    # ---------- Helpers ----------
    def _set_editor_enabled(self, enabled: bool):
        self.editor.config(state="normal" if enabled else "disabled")
        self.editor.config(insertbackground="black" if enabled else "gray")

    def _update_title(self):
        name = self.current_path if self.current_path else "<untitled>"
        self.root.title(f"tKinter — {name}")

    def close_tkinter(self):
        self.root.quit()
        self.root.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = TKinterApp(root)

    if len(sys.argv) > 1:
        arg_path = sys.argv[1]
        try:
            app._load_path(arg_path)
        except Exception as e:
            messagebox.showerror("Launch Error", f"{type(e).__name__}: {e}")

    root.mainloop()
