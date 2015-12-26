#import ctypes

#dll_name = "..\\Release\\ca-lib64.dll"
#ca_lib = ctypes.CDLL(dll_name)
#ca_lib.new_random_seed()
#for i in range(100):
#	print(ca_lib.jkiss_generator(ctypes.c_uint64(10)))

import graphics

g = graphics.gkl_create_graph_pyx(
			graph_width=8, 
			graph_height=8, 
			x_min=0, 
			x_max=0.15, 
			y_min=0, 
			y_max=1, 
			x_axis_title=r"$\alpha$", 
			y_axis_title=r"$d=k/L$",
			axis_title_size=graphics.text.size.large)
points = graphics.gkl_get_data_density(
			file_name=r"C:\Users\Rolf\Documents\GitHub\ca\Release\density_L400.csv",
			state=0)
graphics.gkl_plot_density_map_surface(
			pyx_graph=g, 
			points=points, 
			z_min=-9, 
			z_max=0, 
			z_axis_title=r"log(probability)", 
			axis_title_size=graphics.text.size.large)
graphics.gkl_plot_density_level_curves(
			pyx_graph=g,
			points=points, 
			contour_levels=[-5.0, -3.0, -2.5, -2.0, -1.0])
graphics.gkl_plot_regions_labels(pyx_graph=g)
graphics.gkl_save_graph(
			pyx_graph=g,
			save_to=r"C:\Users\Rolf\Documents\GitHub\ca\Release\gkl4_density_map_surface.pdf")
