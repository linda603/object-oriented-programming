from enum import Enum
from abc import ABC, abstractmethod
from random import random

class ObjectType(Enum):
    DIRECTORY = "DIRECTORY"
    FILE = "FILE"

class FileSystemObject(ABC):
    @abstractmethod
    def get_name(self):
        pass

    @abstractmethod
    def get_parent(self):
        pass

    @abstractmethod
    def get_size(self):
        pass

    @abstractmethod
    def get_type(self):
        pass

    @abstractmethod
    def rename(self, updated_name):
        pass

class File(FileSystemObject):
    def __init__(self, name, parent):
        self.name = name
        self.type = ObjectType.FILE
        self.parent = parent
        self.size = 1000 + int(random() * 10000) # random file size
        self.parent.addObject(self)
        self.content = ""

    def get_name(self):
        return self.name

    def get_parent(self):
        return self.parent

    def get_size(self):
        return self.size

    def get_type(self):
        return self.type

    def rename(self, updated_name):
        self.name = updated_name

    def get_content(self):
        return self.content

    def add_content(self, string):
        self.content += string

class Directory(FileSystemObject):
    def __init__(self, name, parent):
        self.name = name
        self.type = ObjectType.DIRECTORY
        self.parent = parent
        self.size = 0
        self.children = {} # name: FileSystemObject

        if parent:
            self.parent.addObject(self)

    def get_name(self):
        return self.name

    def get_parent(self):
        return self.parent

    def get_size(self):
        return self.size

    def get_type(self):
        return self.type

    def rename(self, updated_name):
        self.name = updated_name

    def get_children(self):
        return self.children

    def get_children_name(self):
        res = []
        for child in self.children.values():
            res.append(child.get_name())
        return sorted(res)

    def addObject(self, object): # object can be file or directory
        if not object:
            raise ValueError("Cannot add null children.")
        self.children[object.get_name()] = object

        # Update curr directory size
        self.update_size(object.get_size())

    def update_size(self, size):
        self.size += size
        if self.parent:
            self.parent.update_size(size)

class FileSystemObjectmanager(ABC):
    @abstractmethod
    def create(self, directory, path):
        pass

class FileManager(FileSystemObjectmanager):
    def create(self, name, parent):
        file = File(name, parent)

class DirectoryManager(FileSystemObjectmanager):
    def create(self, path, parent):
        curr = parent
        for sub_path in path:
            dir = Directory(sub_path, curr)
            curr = dir
        return curr

class FileSystem:
    def __init__(self):
        self.root = Directory("", None)
        self.file_manager = FileManager()
        self.dir_manager = DirectoryManager()

    def process_path(self, path):
        path = path.split("/")
        if path[-1] == "":
            path = path[:-1]
        return path[1:]

    def traverse_path(self, path):
        if not path:
            return [0, self.root]
        curr = self.root

        for i, dir in enumerate(path):
            if dir not in curr.get_children():
                break
            curr = curr.children[dir]
        return [i, curr]

    def ls(self, path):
        arr_path = self.process_path(path)
        i, parent = self.traverse_path(arr_path)
        children_name = parent.get_children_name()
        for child in children_name:
            print(child)
        return

    def mkdir(self, path):
        arr_path = self.process_path(path)
        i, parent = self.traverse_path(arr_path)
        self.dir_manager.create(arr_path[i:], parent)

    def create_file(self, file_path):
        arr_path = self.process_path(file_path)
        i, parent = self.traverse_path(arr_path)
        if parent.get_type() == ObjectType.DIRECTORY:
            dest_dir = self.dir_manager.create(arr_path[i:-1], parent)
            file = self.file_manager.create(arr_path[-1], dest_dir)
        else:
            print("File already created. Please check again.")
            return ""
        return file

    def add_content_to_file(self, file_path, content):
        arr_path = self.process_path(file_path)
        i, object = self.traverse_path(arr_path)
        if object.get_type() == ObjectType.FILE:
            object.add_content(content)
        else:
            dest_dir = self.dir_manager.create(arr_path[i:-1], object)
            file = self.file_manager.create(arr_path[-1], dest_dir)
            file.add_content(content)
        return

    def read_content_from_file(self, file_path):
        arr_path = self.process_path(file_path)
        i, file = self.traverse_path(arr_path)
        if file.get_type() == ObjectType.FILE:
            print(file.get_content())
        else:
            raise ValueError("Invalid file. Please check again")
        return

class Main:
    @staticmethod
    def run():
        file_system = FileSystem()
        file_system.mkdir("/a/b/c")
        file_system.mkdir("/b/d/h")
        file_system.mkdir("/o/p/d")
        file_system.ls("/")
        file_system.create_file("/a/b/c/file1")
        file_system.add_content_to_file("/a/b/c/file1", "testing")
        file_system.read_content_from_file("/a/b/c/file1")
        file_system.create_file("/a/b/c/file1")

if __name__ == "__main__":
    Main.run()