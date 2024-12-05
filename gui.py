import json
import subprocess
import tkinter as tk
from tkinter import filedialog, ttk


class GUI:
    """
    The class for the GUI.
    """

    def __init__(self) -> None:
        root = tk.Tk()
        root.title("Tiny Parser")
        root.geometry("1200x900")

        # The file to parse
        self.file = tk.StringVar(value="test.tiny")

        label1 = ttk.Label(root, text="Chosen file:")
        chosen_file_label = ttk.Label(root, textvariable=self.file)

        chose_file_button = ttk.Button(root, text="Choose File", command=self.open_file)
        parse_button = ttk.Button(root, text="Parse", command=self.parse_file)

        label1.pack()
        chosen_file_label.pack()
        chose_file_button.pack()
        parse_button.pack()

        root.mainloop()

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

    def display_parse_tree(self, json_data):
        print(json_data)


def main():
    GUI()


if __name__ == "__main__":
    main()
