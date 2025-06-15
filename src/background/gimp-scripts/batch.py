#!/usr/bin/env python3

import sys
import gi
gi.require_version('Gimp', '3.0')
from gi.repository import Gimp
gi.require_version('GimpUi', '3.0')
from gi.repository import GimpUi
from gi.repository import GObject
from gi.repository import GLib
from gi.repository import Gtk
from gi.repository import Gio
import sys

sys.stdout.write("TEST\n")

def run(text):

   print("!!!!!!!!!!!!!!!!")

   # result = Gimp.get_pdb().run_procedure(
   #   'gimp-text-layer-new',
   #   [image, text, font_name, font_size, 3]
   # )
   # layer = result.index(1)

    # (image (car (gimp-file-load RUN-NONINTERACTIVE inputFileName inputFileName)))

   print("X-1")
   pdb_proc   = Gimp.get_pdb().lookup_procedure('gimp-file-load')
   print("X-2")
   pdb_config = pdb_proc.create_config()
   pdb_config.set_property('run-mode', Gimp.RunMode.NONINTERACTIVE)
   # pdb_config.set_property('image', "test")
   print("X-3")
   pdb_config.set_property('file', Gio.File.new_for_path("/home/nornetpp/src/rserpooldemo/src/background/Input/Zollverein-small.jpeg"))
   print("X-4")
   result = pdb_proc.run(pdb_config)
   print("X-5")
   print(result.index(0))
   print(result.index(1))
   if (result.index(0) == Gimp.PDBStatusType.SUCCESS):
      print("X-6")
      image = result.index(1)
      print(image)

   # image = Gimp.get_pdb().gimp_file_load("/home/nornetpp/src/rserpooldemo/src/background/Input/Zollverein-small.jpeg", "/home/nornetpp/src/rserpooldemo/src/background/Input/Zollverein-small.jpeg", run_mode=Gimp.RunMode.NONINTERACTIVE)


   print(text)

   # (file-png-save2 RUN-NONINTERACTIVE image drawable outputFileName outputFileName TRUE 9 TRUE TRUE FALSE TRUE TRUE FALSE TRUE)

   print("Y-1")
   pdb_proc   = Gimp.get_pdb().lookup_procedure('file-png-export')
   pdb_config = pdb_proc.create_config()
   pdb_config.set_property('run-mode', Gimp.RunMode.NONINTERACTIVE)
   pdb_config.set_property('image', image)
   pdb_config.set_property('file', Gio.File.new_for_path("x.png"))
   pdb_config.set_property('options', None)
   pdb_config.set_property('interlaced', True)
   pdb_config.set_property('compression', 9)
   # write all PNG chunks except oFFs(ets)
   pdb_config.set_property('bkgd', True)
   pdb_config.set_property('offs', False)
   pdb_config.set_property('phys', True)
   pdb_config.set_property('time', True)
   pdb_config.set_property('save-transparent', True)
   pdb_proc.run(pdb_config)
   print("Y-2")


#if __name__ == "__main__":
print("Running as __main__ with args: %s" % sys.argv)

