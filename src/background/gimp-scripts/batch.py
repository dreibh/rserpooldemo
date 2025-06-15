#!/usr/bin/env python3

import sys
import gi
gi.require_version('Gimp', '3.0')
from gi.repository import Gimp
gi.require_version('Gegl', '0.4')
from gi.repository import Gegl

# gi.require_version('GimpUi', '3.0')
# from gi.repository import GimpUi
# from gi.repository import GObject
# from gi.repository import GLib
# from gi.repository import Gtk
from gi.repository import Gio


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


      layer = image.get_layers()[0]
      print(layer)


          # (gimp-drawable-merge-new-filter drawable "gegl:mosaic" 0 LAYER-MODE-REPLACE 1.0
        # "tile-type" "squares" "tile-size" tileSize "tile-height" tileHeight "tile-neatness" 1 "color-variation" 0 "color-averaging" TRUE "tile-surface" 0 "tile-allow-split" FALSE "tile-spacing" grid-wire-size "joints-color" '(0 0 0) "light-color" '(1 1 1) "light-dir" 135 "antialiasing" TRUE "seed" 0)

      # f = Gimp.DrawableFilter.new(layer, "gegl:gaussian-blur", "")
      # c = f.get_config()
      # c.set_property("std-dev-x", 12.0)
      # c.set_property("std-dev-y", 12.0)
      # c.set_property("clip-extent", 0)
      # f.update()
      # layer.append_filter(f)


      # GEGL Mosaic options: https://gegl.org/operations/gegl-mosaic.html

      f = Gimp.DrawableFilter.new(layer, "gegl:mosaic", "")
      c = f.get_config()
#
#        (plug-in-mosaic RUN-NONINTERACTIVE image drawable
#         tileSize tileHeight tileSpacing
#         0.65 TRUE 135.0 0.2 TRUE TRUE
#         tileType tileSurface FALSE)

      c.set_property("tile-size",       24)
      c.set_property("tile-height",     4)
      c.set_property("tile-spacing",    4)
      c.set_property("tile-neatness",   0.65)
      c.set_property("antialiasing",    True)
      c.set_property("light-dir",       135)
      c.set_property("color-variation", 0.2)
      c.set_property("joints-color",    Gegl.Color.new("rgb(0.0,0.0,0.0)")),
      c.set_property("light-color",     Gegl.Color.new("rgb(1.0,1.0,1.0)")),
      c.set_property("tile-type",       "hexagons")
      c.set_property("tile-surface",    True)
      c.set_property("seed",            0)
      f.update()
      layer.append_filter(f)


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

   del image


#if __name__ == "__main__":
print("Running as __main__ with args: %s" % sys.argv)

