import math
from pyx import *
import pyx_contour

def get_data_recovery_rate(file_name):
	data = []
	with open(file_name) as file:
		perturbation_size = 0
		state = 0
		time = 0
		for row in file:
			if row[0] == '%':
				values = row.split('=')
				if values[0] == "%perturbation_size":
					perturbation_size = int(values[1])
					continue
				elif values[0] == "%state":
					state = int(values[1])
					continue
				elif values[0] == "%time_begin":
					time = int(values[1])
					continue
			else:
				data.append()
				time += 1
	return data

def gkl_get_data_density(file_name,
						 state):
	data = []
	with open(file_name) as file:
		current_state = None
		noise_level = 0
		density = 0
		probability = 0
		CA_size = 1
		for row in file:
			if row == '':
				continue
			elif row[0] == '%':
				values = row.split('=')
				if values[0] == "%state":
					current_state = int(values[1])
					continue
				elif values[0] == "%noise":
					noise_level = float(values[1])
					continue
				elif values[0] == "%state_count_from":
					density = int(values[1])
					continue
				elif values[0] == "%CA_size":
					CA_size = int(values[1])
					continue
			else:
				if current_state != state: continue
				probability = float(row)
				if probability > 0:
					data.append([noise_level, density / CA_size, math.log10(probability)])
				density += 1
		file.close()
	return data

def gkl_create_graph_pyx(graph_width, 
						 graph_height, 
						 x_min, x_max, 
						 y_min, y_max, 
						 x_axis_title, 
						 y_axis_title, 
						 axis_title_size):

	return graph.graphxy(width=graph_width, height=graph_height, 
						x=graph.axis.linear(min=x_min, max=x_max, title=x_axis_title, 
									painter=graph.axis.painter.regular(titleattrs=[axis_title_size]),
									texter=graph.axis.texter.decimal(equalprecision=3),
									manualticks=[graph.axis.tick.tick(0.0, label="0")]),
						y=graph.axis.linear(min=y_min, max=y_max, title=y_axis_title,
									painter=graph.axis.painter.regular(titleattrs=[axis_title_size]),
									texter=graph.axis.texter.decimal(equalprecision=2),
									manualticks=[graph.axis.tick.tick(0.0, label="0")]))

def gkl_plot_density_map_surface(pyx_graph, 
								 points, 
								 z_min, z_max, 
								 z_axis_title, 
								 axis_title_size):

	pyx_graph.plot(graph.data.points(points, x=1, y=2, color=3),
				[graph.style.surface(
							gradient=color.rgbgradient.Jet, backcolor=None, 
							coloraxis=graph.axis.linear(min=z_min, max=z_max, title=z_axis_title,
									   painter=graph.axis.painter.regular(titleattrs=[axis_title_size])))])

def gkl_plot_density_level_curves(pyx_graph, 
								  points, 
								  contour_levels):

	pyx_graph.plot(graph.data.points([[p[0] - 1.0e-2, 
									p[0] + 1.0e-2, 
									p[1] - 1.0e-2, 
									p[1] + 1.0e-2, 
									p[2]] for p in points],
								xmin=1, xmax=2, ymin=3, ymax=4, zval=5),
					[pyx_contour.contourplot(contlevels=contour_levels)])

def gkl_plot_regions_labels(pyx_graph):
	pyx_graph.text(0.0125 * 8 / 0.15, 0.9500 * 8, r"$A$", [text.size.small])
	pyx_graph.text(0.0500 * 8 / 0.15, 0.8125 * 8, r"$B$", [text.size.small])
	pyx_graph.text(0.0625 * 8 / 0.15, 0.4000 * 8, r"$C$", [text.size.small])
	pyx_graph.text(0.1400 * 8 / 0.15, 0.2500 * 8, r"$D$", [text.size.small])
	return pyx_graph

def gkl_save_graph(pyx_graph, save_to):
	pyx_graph.writePDFfile(save_to)
	pass

