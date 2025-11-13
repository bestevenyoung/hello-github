import tkinter as tk
from tkinter import filedialog, messagebox
import os

EXCLUDE_PATTERNS = [
    "venv", "__pycache__", ".egg-info", ".dist-info",
    "site-packages", "bin", "lib", "share", "include",
    "*.pyc", "*.pyo", "*.so", "*.dll", "*.dylib",
    "*.txt", "*.md", "*.rst", "*.typed", "tests", "test", "docs"
]

INCLUDE_CONTENTS = (".py", ".json")
SHOW_ONLY = (".pdf", ".docx", ".odt")

def should_exclude(name):
    import fnmatch
    return any(fnmatch.fnmatch(name, pat) for pat in EXCLUDE_PATTERNS)

def build_tree(root_path, prefix=""):
    """Return a list of tree lines and a list of file paths to dump."""
    lines, files = [], []
    entries = sorted(os.listdir(root_path))
    for i, entry in enumerate(entries):
        path = os.path.join(root_path, entry)
        connector = "└── " if i == len(entries)-1 else "├── "
        if os.path.isdir(path):
            if should_exclude(entry):
                lines.append(prefix + connector + entry + "/ (excluded)")
                continue
            lines.append(prefix + connector + entry + "/")
            sub_prefix = prefix + ("    " if i == len(entries)-1 else "│   ")
            sub_lines, sub_files = build_tree(path, sub_prefix)
            lines.extend(sub_lines)
            files.extend(sub_files)
        else:
            if should_exclude(entry):
                continue
            lines.append(prefix + connector + entry)
            if entry.endswith(INCLUDE_CONTENTS):
                files.append(path)
    return lines, files

def snapshot_project(path, out_file):
    tree_lines, files = build_tree(path)
    with open(out_file, "w", encoding="utf-8") as f:
        f.write("\n".join(tree_lines))
        f.write("\n\n")
        for file in files:
            rel = os.path.relpath(file, path)
            f.write(f"<file: {rel}>\n")
            try:
                with open(file, "r", encoding="utf-8") as src:
                    f.write(src.read())
            except Exception as e:
                f.write(f"[Error reading file: {e}]\n")
            f.write(f"\n/<{os.path.basename(file)}>\n\n")

class SnapshotApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Project Snapshotter")
        tk.Button(root, text="Select Project Folder", command=self.select_folder).pack(pady=20)

    def select_folder(self):
        folder = filedialog.askdirectory()
        if not folder:
            return
        project_name = os.path.basename(folder.rstrip(os.sep))
        out_file = filedialog.asksaveasfilename(
            initialfile=f"{project_name}.txt",
            defaultextension=".txt",
            filetypes=[("Text Files", "*.txt")]
        )
        if not out_file:
            return
        try:
            snapshot_project(folder, out_file)
            messagebox.showinfo("Success", f"Snapshot saved to {out_file}")
        except Exception as e:
            messagebox.showerror("Error", str(e))

if __name__ == "__main__":
    root = tk.Tk()
    app = SnapshotApp(root)
    root.mainloop()
