import json
import subprocess
import tkinter as tk
from tkinter import filedialog, ttk

import graphviz

TERMINALS = [
    "OP",
    "CONST",
    "ID",
]


class GUI:
    """
    The class for the GUI.
    """

    def __init__(self) -> None:
        self.root = tk.Tk()
        self.root.title("Tiny Parser")
        self.root.geometry("1200x900")
        self.dot = graphviz.Digraph(comment="Parse Tree")
        self.count = 0

        # The file to parse
        self.file = tk.StringVar(value="tests/test.tiny")

        label1 = ttk.Label(self.root, text="Chosen file:")
        chosen_file_label = ttk.Label(self.root, textvariable=self.file)

        chose_file_button = ttk.Button(
            self.root, text="Choose File", command=self.open_file
        )
        parse_button = ttk.Button(self.root, text="Parse", command=self.parse_file)

        label1.pack()
        chosen_file_label.pack()
        chose_file_button.pack()
        parse_button.pack()

        self.root.mainloop()

    def open_file(self):
        file_path = filedialog.askopenfilename()
        self.file.set(file_path)

    def parse_file(self):
        file_path = self.file.get()
        try:
            subprocess.run(["./build/parser", file_path], check=True)
            with open("parse_tree.json", "r", encoding="utf-8") as json_file:
                json_data = json.load(json_file)
                self.display_parse_tree(json_data)
        except subprocess.CalledProcessError as e:
            print(f"Error running parser: {e}")

    def add_subtree(self, json_data):
        if not json_data:
            return None

        current_node_id = str(self.count)
        self.count += 1
        current_node_type = json_data.get("type", "")[5:]
        current_node_value = json_data.get("value", "")
        current_node_text = f"{current_node_id}\n{current_node_type}\n{current_node_value}"

        current_node_shape = shape(current_node_type)
        self.dot.node(current_node_text, shape=current_node_shape)

        for key in ["left", "right"]:
            child_node_text = self.add_subtree(json_data.get(key))
            if child_node_text:
                self.dot.edge(current_node_text, child_node_text)

        if json_data.get("next"):
            # draw the next node as a sibling
            next_node_text = self.add_subtree(json_data["next"])
            self.dot.edge(current_node_text, next_node_text)
            with self.dot.subgraph() as s:
                s.attr(rank="same")
                s.node(current_node_text)
                s.node(next_node_text)

        return current_node_text

    def display_parse_tree(self, json_data):
        print(json_data["value"])
        self.add_subtree(json_data)
        self.dot.render("parse_tree", format="png", view=True)


def shape(node_type: str) -> str:
    if node_type in TERMINALS:
        return "ellipse"
    return "box"


def main():
    GUI()


if __name__ == "__main__":
    main()
