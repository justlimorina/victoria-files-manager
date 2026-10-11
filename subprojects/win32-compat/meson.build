project('win32-compat', 'c', version: '44.0', default_options: ['c_std=c11'])

glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
gio_dep = dependency('gio-2.0')
gdk_pixbuf_dep = dependency('gdk-pixbuf-2.0')
gtk_dep = dependency('gtk4')
gsettings_schemas_dep = dependency('gsettings-desktop-schemas')

compat_inc = include_directories('include')

# 1. gnome-desktop-4 shim
libgnome_desktop_compat = static_library(
  'gnome-desktop-compat',
  'gnome-desktop-thumbnail.c',
  include_directories: compat_inc,
  dependencies: [glib_dep, gobject_dep, gio_dep, gdk_pixbuf_dep, gsettings_schemas_dep]
)
gnome_desktop_dep = declare_dependency(
  link_with: libgnome_desktop_compat,
  include_directories: compat_inc,
  dependencies: [glib_dep, gobject_dep, gio_dep, gdk_pixbuf_dep, gsettings_schemas_dep]
)
meson.override_dependency('gnome-desktop-4', gnome_desktop_dep)

# 2. libportal & libportal-gtk4 shim
libportal_compat = static_library(
  'portal-compat',
  'portal-compat.c',
  include_directories: compat_inc,
  dependencies: [glib_dep, gobject_dep, gio_dep, gtk_dep]
)
portal_dep = declare_dependency(
  link_with: libportal_compat,
  include_directories: compat_inc,
  dependencies: [glib_dep, gobject_dep, gio_dep, gtk_dep]
)
portal_gtk4_dep = portal_dep
meson.override_dependency('libportal', portal_dep)
meson.override_dependency('libportal-gtk4', portal_gtk4_dep)

# 3. tracker-sparql-3.0 shim
libtracker_compat = static_library(
  'tracker-compat',
  'tracker-compat.c',
  include_directories: compat_inc,
  dependencies: [glib_dep, gobject_dep, gio_dep]
)
tracker_dep = declare_dependency(
  link_with: libtracker_compat,
  include_directories: compat_inc,
  dependencies: [glib_dep, gobject_dep, gio_dep]
)
meson.override_dependency('tracker-sparql-3.0', tracker_dep)
