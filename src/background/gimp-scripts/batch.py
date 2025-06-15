#!/usr/bin/env python3

import sys
import gi
gi.require_version('Gimp', '3.0')
from gi.repository import Gimp
gi.require_version('Gegl', '0.4')
from gi.repository import Gegl
from gi.repository import Gio


# ====== Load image =========================================================
pdb_proc   = Gimp.get_pdb().lookup_procedure('gimp-file-load')
pdb_config = pdb_proc.create_config()
pdb_config.set_property('run-mode', Gimp.RunMode.NONINTERACTIVE)
pdb_config.set_property('file',     Gio.File.new_for_path("/home/nornetpp/src/rserpooldemo/src/background/Input/Zollverein-small.jpeg"))
result = pdb_proc.run(pdb_config)
if (result.index(0) == Gimp.PDBStatusType.SUCCESS):

   # ====== Prepare filter usage ============================================
   image = result.index(1)
   layer = image.get_layers()[0]


   # ====== Apply Mosaic filter =============================================

   # GEGL Mosaic options: https://gegl.org/operations/gegl-mosaic.html

   f = Gimp.DrawableFilter.new(layer, "gegl:mosaic", "")
   c = f.get_config()

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


   # ====== Save result =====================================================
   pdb_proc   = Gimp.get_pdb().lookup_procedure('file-png-export')
   pdb_config = pdb_proc.create_config()
   pdb_config.set_property('run-mode',         Gimp.RunMode.NONINTERACTIVE)
   pdb_config.set_property('image',            image)
   pdb_config.set_property('file',             Gio.File.new_for_path("x.png"))
   pdb_config.set_property('options',          None)
   pdb_config.set_property('interlaced',       True)
   pdb_config.set_property('compression',      9)
   # write all PNG chunks except oFFs(ets)
   pdb_config.set_property('bkgd',             True)
   pdb_config.set_property('offs',             False)
   pdb_config.set_property('phys',             True)
   pdb_config.set_property('time',             True)
   pdb_config.set_property('save-transparent', True)
   result =  pdb_proc.run(pdb_config)
   print(result)


   # ====== Clean up ========================================================
   Gimp.Image.delete(image)
