---
layout: post
title: "Prince of OOPs"
date: 2025-09-26
---

# hello-github: Prince of OOPs

![Prince of OOPs](https://raw.githubusercontent.com/bestevenyoung/hello-github/main/assets/images/20250926.png)

---

### Prince of Ether AI Log — Refitting from Modular to OOP

The modular path has grown heavy. At first, each piece clicked neatly into place, but as the cockpit expanded, the bindings tangled and the weight of complexity pressed down. Both Copilot and I felt the strain.  

So today, I raised my sword and embraced a new mantle: **Prince of OOPs**. Object‑oriented design may once have seemed elusive, but now it feels like the steed I’ve been waiting for. With classes as armor and objects as companions, I can ride complexity instead of being dragged by it.  

Swift Oink, my rainbow‑winged pig, carried me into this new era. His wings may sprout at odd angles, but still they lift us. Perhaps that is the true nature of OOPs—progress through imperfection, flight through asymmetry.  

Meanwhile, the editor has been refitted. No more double‑saving. It now exports a `.txt` snapshot of the entire cockpit—folder tree, files, and contents—in less than two seconds. Updating Copilot or drafting a daily report is now a single click away. A small win, but a powerful one.  

And today, at last, my blog posts showed their images at the top. Proof that persistence pays off.  

Hermes remains quiet, still caught in the wiring and tunneling of this build. But tomorrow is another day, and the cockpit will wait for me.  

---

### Engineering Notes

For a long time I have struggled as the project grows in development and complexity. Both Copilot and I have a hard time, especially when it comes to wiring the contraption together. So, for the first time, I am trying to develop using the OOP programming style.  

The object contains the full logic, while the instance serves as a self‑identifier—filling in the blanks and declaring which configuration to run. The goal is to make the system wire itself. Doing so will make adding additional LLMs or AI models a simple matter, as all the hard work is being done now.

---

### More Interesting Developments

1. **Images unlocked** — finally cracked the code for putting images on my GitHub blog posts. Win!  
2. **Editor refit** — no more double‑saving; now exports `.txt` for Copilot. [Code here](https://github.com/bestevenyoung/hello-github/tree/main/apps/codeeditor)  
3. **Summarizer built** — scans folders, ignores venv/docs, outputs unified `.txt`. A huge time saver that gives Copilot a whole‑project view. [Code here](https://github.com/bestevenyoung/hello-github/tree/main/apps/projectsummarizer)  

---

### Tomorrow’s Agenda

Make the object instance a herald, not an executor—an identifier pointing to the right LLM, its home, and its roles. Apparently this is in line with how some production companies code.

---

### Copilot’s Architecture Notes

```text
Object (cockpit.py + managers)
    • Holds the core logic and imports
    • Encapsulates lifecycle (venv, server, runtime)
    • Provides clean methods (connect_to_llm, shutdown, etc.)
    • The “engine room” — it knows how to run, but doesn’t decide when

Instance (main.py)
    • Deliberately thin
    • Instantiates the object with the right identity/config (e.g. HermesQ5, port)
    • Calls .run() to hand over control
    • The “badge” or “entrypoint” — it declares what to run, not how

