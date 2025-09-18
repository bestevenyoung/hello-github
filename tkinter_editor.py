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

        # Buttons (after editor exists is fine; methods are class-bound)
        for text, cmd in [
            ("Run",           self.run_code),
            ("Save",          self.save_code),
            ("Load",          self.open_code),
            ("Close Code",    self.close_code),   # clear + disable editor
            ("Reset Editor",  self.reset_editor), # destroy + rebuild frame+editor
            ("Exit",          self.close_tkinter),
        ]:
            ttk.Button(self.toolbar, text=text, command=cmd).pack(fill="x", pady=3)

        self.current_path = None
        self._update_title()

    # ---------- Editor construction / reset ----------
    def _build_editor(self):
        # Fresh Text widget inside editor_frame
        self.editor = tk.Text(self.editor_frame, wrap="none", undo=True)
        self.editor.pack(fill="both", expand=True)

        # Clipboard + focus bindings
        self.editor.bind("<Control-c>", lambda e: self.editor.event_generate("<<Copy>>"))
        self.editor.bind("<Control-v>", lambda e: self.editor.event_generate("<<Paste>>"))
        self.editor.bind("<Control-x>", lambda e: self.editor.event_generate("<<Cut>>"))
        self.editor.bind("<Button-1>", lambda e: self.editor.focus_set())

    def reset_editor(self):
        """Destroy and recreate the editor frame and Text cleanly."""
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
        # Ask for path if we don't have one yet
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
        """
        Launch the target as an independent Tkinter app.
        - No capture panes.
        - No dummy '<no output>' dialogs.
        - Only show a dialog if launch fails.
        """
        target = self.current_path or RUN_DEFAULT

        # Normalize and check existence before launching
        if not os.path.isabs(target):
            target = os.path.abspath(target)

        if not os.path.exists(target):
            messagebox.showerror("Run Error", f"File not found:\n{target}")
            return

        try:
            # Fire-and-forget: let the target open its own window and mainloop.
            subprocess.Popen([sys.executable, target])
            # No messagebox on success—keep it quiet and simple.
        except Exception as e:
            messagebox.showerror("Run Error", f"{type(e).__name__}: {e}")

    # ---------- Close / disable editor ----------
    def close_code(self):
        """
        Clear and disable the editor without touching layout.
        This keeps the right column stable and avoids toolbar reflow.
        """
        self._set_editor_enabled(True)
        self.editor.delete("1.0", "end")
        self._set_editor_enabled(False)
        self.current_path = None
        self._update_title()

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

    # Accept a filename passed on the command line (e.g., `code somefile.py`)
    if len(sys.argv) > 1:
        arg_path = sys.argv[1]
        try:
            app._load_path(arg_path)
        except Exception as e:
            messagebox.showerror("Launch Error", f"{type(e).__name__}: {e}")

    root.mainloop()
