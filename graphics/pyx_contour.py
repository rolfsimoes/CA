import math, sys
from pyx import *

#
# pyx_contour.py : version 5
# 
######################################################################################################
	
class contourplot(graph.style._style):

	needsdata = ["vrange", "vrangeminmissing", "vrangemaxmissing"]

	def __init__(self,contlevels=[],dolegend=True,logscal=False,linestyle=[color.rgb.black],tol=0.05,
			label=False,label_list=[],label_format="%10.3e",label_dist=1.0,
			label_border=0.5,**kwargs):
		if len(contlevels) < 1:
			raise ValueError("no value for contourplot given")
		self.contlevels=contlevels
		self.dolegend=dolegend
		self.datapoints={}
		self.logscal=logscal
		self.linestyle=linestyle
		self.tol=tol
		self.label=label
		if label_list==[]:
			self.label_list=range(len(contlevels))
		else:
			self.label_list=label_list
		self.label_format=label_format
		self.label_dist=label_dist
		self.label_border=label_border

	def columnnames(self, privatedata, sharedata, graph, columnnames, dataaxisnames):
		if len(graph.axesnames) != 2:
			raise TypeError("rect style restricted on two-dimensional graphs")
		if len(sharedata.vrangeminmissing) + len(sharedata.vrangemaxmissing):
			raise ValueError("incomplete range")
		if "zval" not in columnnames:
			raise ValueError("need zval")
		return ["zval"]			

	def adjustaxis(self, privatedata, sharedata, graph, plotitem, columnname, data):
		self.datapoints[columnname]=data

	def do_contplot(self, privatedata, sharedata, graph):

		# need to build a 2d grid for the contour plot-algorithm

		# find all different axis-point (already in the graph-coordinates
		# to avoid problems with logarithmic axis)
		# this implementation takes care of small differences, i.e.
		# points closer than "tol" are ignored / collected

		nrpoints=len(self.datapoints["xmin"])

		xaxis=[]
		yaxis=[]
		
		xdat=[]
		ydat=[]
		zdat=[]

		for i in range(nrpoints):
			x_min,y_min=graph.pos(self.datapoints["xmin"][i],
						self.datapoints["ymin"][i])
			x_max,y_max=graph.pos(self.datapoints["xmax"][i],
						self.datapoints["ymax"][i])
			xavg=0.5*(x_max+x_min)
			yavg=0.5*(y_min+y_max)

			if not xavg in xaxis: xaxis.append(xavg)
			if not yavg in yaxis: yaxis.append(yavg)

			xdat.append(xavg)
			ydat.append(yavg)

			if self.logscal:
				if self.datapoints["zval"][i]!=None:
					zval=math.log10(self.datapoints["zval"][i])
				else:
					zval=None	
			else:
				zval=self.datapoints["zval"][i]

			zdat.append(zval)

		xaxis.sort()
		yaxis.sort()
		xtemp=[xaxis[0]]
		ytemp=[yaxis[0]]
		
		for xv in xaxis:
			if xv >= xtemp[-1]+self.tol:
				xtemp.append(xv)
		for yv in yaxis:
			if yv >= ytemp[-1]+self.tol:
				ytemp.append(yv)

		xaxis=xtemp
		yaxis=ytemp

		i_xaxis=len(xaxis)
		i_yaxis=len(yaxis)

		datagrid=[[None for i in range(i_yaxis)] for j in range(i_xaxis)]

		zmax=max(zdat)
		zmin=min(zdat)

		for ip in range(nrpoints):

			xp=xdat[ip]
			yp=ydat[ip]

			dist=9e99
			for ix_s in range(i_xaxis):
				d=abs(xp-xaxis[ix_s])
				if d < dist:
					dist=d
					ix=ix_s

			dist=9e99
			for iy_s in range(i_yaxis):
				d=abs(yp-yaxis[iy_s])
				if d < dist:
					dist=d
					iy=iy_s

			datagrid[ix][iy]=zdat[ip]

		# don't make a mess...
		p=path.rect(graph.xpos,graph.ypos,graph.width,graph.height)
		doclip=canvas.canvas([canvas.clip(p)])

		# do the rendering for every rectangle for each contour-level
		
		i_level=1
		for val in self.contlevels:
			if self.logscal:
				cvalue=math.log10(val)
			else:
				cvalue=val

			if cvalue >= zmin and cvalue < zmax:

				list_pts=[]

				for ix in range(i_xaxis-1):
					for iy in range(i_yaxis-1):
						x1=xaxis[ix]
						x2=xaxis[ix+1]
						y1=yaxis[iy]
						y2=yaxis[iy+1]
					
						if datagrid[ix][iy]!=None and datagrid[ix+1][iy]!=None and \
							datagrid[ix][iy+1]!=None and datagrid[ix+1][iy+1]!=None:
							self.render_triangle(x1,y1,x2,y1,x2,y2,
								datagrid[ix][iy],
								datagrid[ix+1][iy],
								datagrid[ix+1][iy+1],cvalue,doclip,list_pts)
							self.render_triangle(x1,y1,x2,y2,x1,y2,
								datagrid[ix][iy],
								datagrid[ix+1][iy+1],
								datagrid[ix][iy+1],cvalue,doclip,list_pts)

				if self.label and i_level in self.label_list:
					point=[]
					for line in list_pts:
						dx=(line[2]-line[0])
						if abs(dx) < 1.e-8:
							dx=1e-8
						dy=(line[3]-line[1])
						slope=math.atan(dy/dx)*180.0/math.pi
						mid_x=0.5*(line[2]+line[0])
						mid_y=0.5*(line[3]+line[1])
						point.append([mid_x,mid_y,slope])

					take=[]
					for pt in point:
						dist=9e99
						for pts in take:
							dist=min(dist,math.sqrt((pt[0]-pts[0])**2+(pt[1]-pts[1])**2))
						if dist > self.label_dist:
							take.append(pt)

					for pt in take:
						dist=min(abs(pt[0]-graph.xpos),abs(pt[0]-(graph.xpos+graph.width)),
							 abs(pt[1]-graph.ypos),abs(pt[1]-(graph.ypos+graph.height)))

						if dist > self.label_border:

							text_w=self.label_format % val
							tbox=text.text(pt[0],pt[1],text_w,[text.halign.center,text.valign.middle]) #,text.size.tiny])
							tpath = tbox.bbox().enlarged(1*unit.x_pt).path()
							doclip.draw(tpath,[deco.filled([color.rgb.white]),trafo.rotate(pt[2],x=pt[0],y=pt[1])])
							doclip.insert(tbox,[trafo.rotate(pt[2],x=pt[0],y=pt[1])])

			i_level+=1

		# insert the "cliped" canvas

		graph.insert(doclip)

	def lin_interpol(self, x0,x1,y0,y1,xint):
		alpha=(xint-x0)/(x1-x0)
		return y0*(1.0-alpha)+alpha*y1

	def render_triangle(self,x0,y0,x1,y1,x2,y2,v0,v1,v2,lval,mycanv,list_pts):

		line=[0.0,0.0,0.0,0.0]
		draw=False

		p0=v0 > lval
		p1=v1 > lval
		p2=v2 > lval

		# will not win a beauty contest...
		if p0==False and p1==False and p2==False:
			pass
		if p0==True and p1==True and p2==True:
			pass
		if p0==True and p1==False and p2==False:
			draw=True
			xc1=self.lin_interpol(v0,v1,x0,x1,lval)
			yc1=self.lin_interpol(v0,v1,y0,y1,lval)
			xc2=self.lin_interpol(v0,v2,x0,x2,lval)
			yc2=self.lin_interpol(v0,v2,y0,y2,lval)
			line=[xc1,yc1,xc2,yc2]
		if p0==False and p1==True and p2==False:
			draw=True
			xc1=self.lin_interpol(v1,v2,x1,x2,lval)
			yc1=self.lin_interpol(v1,v2,y1,y2,lval)
			xc2=self.lin_interpol(v0,v1,x0,x1,lval)
			yc2=self.lin_interpol(v0,v1,y0,y1,lval)
			line=[xc1,yc1,xc2,yc2]
		if p0==False and p1==False and p2==True:
			draw=True
			xc1=self.lin_interpol(v1,v2,x1,x2,lval)
			yc1=self.lin_interpol(v1,v2,y1,y2,lval)
			xc2=self.lin_interpol(v0,v2,x0,x2,lval)
			yc2=self.lin_interpol(v0,v2,y0,y2,lval)
			line=[xc1,yc1,xc2,yc2]
		if p0==False and p1==True and p2==True:
			draw=True
			xc1=self.lin_interpol(v0,v1,x0,x1,lval)
			yc1=self.lin_interpol(v0,v1,y0,y1,lval)
			xc2=self.lin_interpol(v0,v2,x0,x2,lval)
			yc2=self.lin_interpol(v0,v2,y0,y2,lval)
			line=[xc1,yc1,xc2,yc2]
		if p0==True and p1==False and p2==True:
			draw=True
			xc1=self.lin_interpol(v1,v2,x1,x2,lval)
			yc1=self.lin_interpol(v1,v2,y1,y2,lval)
			xc2=self.lin_interpol(v0,v1,x0,x1,lval)
			yc2=self.lin_interpol(v0,v1,y0,y1,lval)
			line=[xc1,yc1,xc2,yc2]
		if p0==True and p1==True and p2==False:
			draw=True
			xc1=self.lin_interpol(v1,v2,x1,x2,lval)
			yc1=self.lin_interpol(v1,v2,y1,y2,lval)
			xc2=self.lin_interpol(v0,v2,x0,x2,lval)
			yc2=self.lin_interpol(v0,v2,y0,y2,lval)
			line=[xc1,yc1,xc2,yc2]

		if draw:
			list_pts.append(line)
			mycanv.stroke(path.line(line[0],line[1],line[2],line[3]),self.linestyle)

	def initdrawpoints(self, privatedata, sharedata, graph):
		try:
			a=sharedata.gaxis
			gaxis_ex=True
		except:
			gaxis_ex=False
		if self.dolegend and gaxis_ex:
			minax=sharedata.gaxis.axes["y2"].axis.min
			maxax=sharedata.gaxis.axes["y2"].axis.max
			c=canvas.canvas()
			for lev in self.contlevels:
				if lev >=minax and lev<=maxax:
					xa,ya=sharedata.gaxis.pos(0.0,lev)
					xb,yb=sharedata.gaxis.pos(1.0,lev)
					c.stroke(path.line(xa,ya,xb,yb),self.linestyle)
			graph.layer("data").insert(c)			

		self.do_contplot(privatedata, sharedata, graph)

	def drawpoint(self, privatedata, sharedata, graph, point):
		pass

	def key_pt(self, privatedata, sharedata, graph, x_pt, y_pt, width_pt, height_pt):
		raise RuntimeError("Style currently doesn't provide a graph key")	

######################################################################################################

class gridplot(graph.style._style):

	needsdata = ["vrange", "vrangeminmissing", "vrangemaxmissing"]

	def __init__(self,skipnone=False,linestyle=[color.rgb.black,style.linewidth(0.01)],**kwargs):
		self.datapoints={}
		self.skipnone=skipnone
		self.linestyle=linestyle

	def columnnames(self, privatedata, sharedata, graph, columnnames, dataaxisnames):
		if len(graph.axesnames) != 2:
			raise TypeError("rect style restricted on two-dimensional graphs")
		if len(sharedata.vrangeminmissing) + len(sharedata.vrangemaxmissing):
			raise ValueError("incomplete range")
		if "zval" not in columnnames:
			raise ValueError("need zval")
		return ["zval"]			

	def adjustaxis(self, privatedata, sharedata, graph, plotitem, columnname, data):
		self.datapoints[columnname]=data	

	def initdrawpoints(self, privatedata, sharedata, graph):
		
		p=path.rect(graph.xpos,graph.ypos,graph.width,graph.height)
		doclip=canvas.canvas([canvas.clip(p)])
		
		nrpoints=len(self.datapoints["xmin"])
		for i in range(nrpoints):
			if not (self.skipnone and self.datapoints["zval"][i]==None):
				xmin,ymin=graph.pos(self.datapoints["xmin"][i],self.datapoints["ymin"][i])
				xmax,ymax=graph.pos(self.datapoints["xmax"][i],self.datapoints["ymax"][i])
				doclip.stroke(path.rect(xmin,ymin,xmax-xmin,ymax-ymin),self.linestyle)				
				
		graph.layer("data").insert(doclip)			
		

	def drawpoint(self, privatedata, sharedata, graph, point):
		pass

	def key_pt(self, privatedata, sharedata, graph, x_pt, y_pt, width_pt, height_pt):
		raise RuntimeError("Style currently doesn't provide a graph key")

######################################################################################################

class fillplot(graph.style._style):

	needsdata = ["vrange", "vrangeminmissing", "vrangemaxmissing"]

	def __init__(self,min=None,max=None,logscal=False,dolegende=False,
			legendegap=0.0,legendesize=None,legendtext=None,legendtextattr=[text.size.large],
			legendtextdist=1.0,gradient=color.gradient.Grey,**kwargs):
		self.minval=min
		self.maxval=max
		self.logscal=logscal
		self.dolegende=dolegende
		self.legendegap=legendegap
		self.legendesize=legendesize
		self.legendtext=legendtext
		self.legendtextattr=legendtextattr
		self.legendtextdist=legendtextdist			
		self.gradient=gradient

	def columnnames(self, privatedata, sharedata, graph, columnnames, dataaxisnames):
		if len(graph.axesnames) != 2:
			raise TypeError("rect style restricted on two-dimensional graphs")
		if len(sharedata.vrangeminmissing) + len(sharedata.vrangemaxmissing):
			raise ValueError("incomplete range")
		if "zval" not in columnnames:
			raise ValueError("need zval")		
		return ["zval"]			
		
	def initdrawpoints(self, privatedata, sharedata, tgraph):
		privatedata.rectcanvas = tgraph.layer("filldata").insert(canvas.canvas())		
		
		if self.dolegende:
			if self.logscal:
				valax_plot=graph.axis.log(min=self.minval,max=self.maxval)
			else:
				valax_plot=graph.axis.lin(min=self.minval,max=self.maxval)

			if self.legendesize==None:
				leg_wdt=tgraph.width/10.0
			else:
				leg_wdt=self.legendesize

			gaxis=graph.graphxy(height=tgraph.height,width=leg_wdt,
					xpos=tgraph.xpos+tgraph.width+self.legendegap,
					ypos=tgraph.ypos,
					x=graph.axis.linear(min=0.0,max=1.0,parter=None),
					y2=valax_plot)
					
			nr_index_cols=256			
			c=canvas.canvas()			
			for i in range(nr_index_cols):
				if self.logscal:
					ind_l=self.minval*10**(i*(math.log10(self.maxval)-math.log10(self.minval))/nr_index_cols)
					ind_h=self.minval*10**((i+1)*(math.log10(self.maxval)-math.log10(self.minval))/nr_index_cols)
				else:
					ind_l=self.minval+i*(self.maxval-self.minval)/nr_index_cols
					ind_h=self.minval+(i+1)*(self.maxval-self.minval)/nr_index_cols
				alpha=i/(nr_index_cols-1.0)				
				x1,y1=gaxis.pos(0.0,ind_l); x2,y2=gaxis.pos(1.0,ind_h)		
				c.fill(path.rect(x1,y1,x2-x1,y2-y1),[self.gradient.getcolor(alpha)])				
			gaxis.layer("background").insert(c)	
						
			if self.legendtext!=None:		
				x_legtext=tgraph.xpos+tgraph.width+self.legendegap+leg_wdt+self.legendtextdist
				y_legtext=tgraph.ypos+0.5*tgraph.height				
				textpos=self.legendtextattr
				textpos.extend([text.halign.center,text.valign.middle,
						trafo.rotate(-90.0),trafo.translate(x_legtext,y_legtext)])
				gaxis.text(0.0,0.0,self.legendtext,textpos)
											
			sharedata.gaxis=gaxis	
			tgraph.layer("background").insert(gaxis)
			
	def adjustaxis(self, privatedata, sharedata, graph, plotitem, columnname, data):
		if columnname=="zval":
			if self.minval==None:
				self.minval=min(x for x in data if x is not None)												
			if self.maxval==None:
				self.maxval=max(x for x in data if x is not None)	

	def drawpoint(self, privatedata, sharedata, graph, point):
		xvmin = sharedata.vrange[0][0]
		xvmax = sharedata.vrange[0][1]
		yvmin = sharedata.vrange[1][0]
		yvmax = sharedata.vrange[1][1]
		if (xvmin is not None and xvmin < 1 and
			xvmax is not None and xvmax > 0 and
			yvmin is not None and yvmin < 1 and
			yvmax is not None and yvmax > 0):
			if xvmin < 0:
				xvmin = 0
			elif xvmax > 1:
				xvmax = 1
			if yvmin < 0:
				yvmin = 0
			elif yvmax > 1:
				yvmax = 1
			p = graph.vgeodesic(xvmin, yvmin, xvmax, yvmin)
			p.append(graph.vgeodesic_el(xvmax, yvmin, xvmax, yvmax))
			p.append(graph.vgeodesic_el(xvmax, yvmax, xvmin, yvmax))
			p.append(graph.vgeodesic_el(xvmin, yvmax, xvmin, yvmin))
			p.append(path.closepath())
			pvalue=point["zval"]
			if pvalue==None:
				cvalue=-1.0
			else:				
				if not self.logscal:
					cvalue=(pvalue-self.minval)/(self.maxval-self.minval)
				else:
					try:
						cvalue=(math.log10(pvalue)-math.log10(self.minval))/ \
							(math.log10(self.maxval)-math.log10(self.minval))
					except:
						cvalue=-1.0
			if cvalue >=0.0 and cvalue <=1.0:
				privatedata.rectcanvas.fill(p, [self.gradient.getcolor(cvalue)])

	def key_pt(self, privatedata, sharedata, graph, x_pt, y_pt, width_pt, height_pt):
		raise RuntimeError("Style currently doesn't provide a graph key")

######################################################################################################

class fillplot_bmp(graph.style._style):

	needsdata = ["vrange", "vrangeminmissing", "vrangemaxmissing"]

	def __init__(self,min=None,max=None,logscal=False,dolegende=False,
			legendegap=0.0,legendesize=None,legendtext=None,legendtextattr=[text.size.large],
			legendtextdist=1.0,gradient=color.gradient.Grey,x_bmp=100,y_bmp=100,**kwargs):
		self.minval=min
		self.maxval=max
		self.logscal=logscal
		self.dolegende=dolegende
		self.datapoints={}
		self.legendegap=legendegap
		self.legendesize=legendesize
		self.legendtext=legendtext
		self.legendtextattr=legendtextattr
		self.legendtextdist=legendtextdist		
		self.x_bmp=x_bmp
		self.y_bmp=y_bmp
		self.gradient=gradient
		
	def columnnames(self, privatedata, sharedata, graph, columnnames, dataaxisnames):
		if len(graph.axesnames) != 2:
			raise TypeError("rect style restricted on two-dimensional graphs")
		if len(sharedata.vrangeminmissing) + len(sharedata.vrangemaxmissing):
			raise ValueError("incomplete range")
		if "zval" not in columnnames:
			raise ValueError("need zval")	
		return ["zval"]			

	def initdrawpoints(self, privatedata, sharedata, tgraph):
		privatedata.rectcanvas = tgraph.layer("filldata").insert(canvas.canvas())		

		if self.dolegende:
			if self.logscal:
				valax_plot=graph.axis.log(min=self.minval,max=self.maxval)
			else:
				valax_plot=graph.axis.lin(min=self.minval,max=self.maxval)

			if self.legendesize==None:
				leg_wdt=tgraph.width/10.0
			else:
				leg_wdt=self.legendesize

			gaxis=graph.graphxy(height=tgraph.height,width=leg_wdt,
					xpos=tgraph.xpos+tgraph.width+self.legendegap,
					ypos=tgraph.ypos,
					x=graph.axis.linear(min=0.0,max=1.0,parter=None),
					y2=valax_plot)

			nr_index_cols=255
			index_str=""
			for i in range(nr_index_cols):
				alpha=1.0-i/(nr_index_cols-1.0)
				col=self.gradient.getcolor(alpha).rgb().color
				col_str=chr(int(255.0*col["r"]))+chr(int(255.0*col["g"]))+chr(int(255.0*col["b"]))
				index_str+=col_str

			image_rgb_ind=bitmap.image(1,nr_index_cols,"RGB",index_str)
			
			bitmap_rgb_ind=bitmap.bitmap(tgraph.xpos+tgraph.width+self.legendegap,tgraph.ypos,
						image_rgb_ind,height=tgraph.height,width=leg_wdt)
			privatedata.rectcanvas.insert(bitmap_rgb_ind)

			sharedata.gaxis=gaxis
			
			if self.legendtext!=None:		
				x_legtext=tgraph.xpos+tgraph.width+self.legendegap+leg_wdt+self.legendtextdist
				y_legtext=tgraph.ypos+0.5*tgraph.height				
				textpos=self.legendtextattr
				textpos.extend([text.halign.center,text.valign.middle,
						trafo.rotate(-90.0),trafo.translate(x_legtext,y_legtext)])
				gaxis.text(0.0,0.0,self.legendtext,textpos)
			
			privatedata.rectcanvas.insert(gaxis)


		xsize=self.x_bmp
		ysize=self.y_bmp

		pic=[3*chr(255) for i in range(xsize*ysize)]

		tgraph_xpos=tgraph.xpos
		tgraph_ypos=tgraph.ypos
		tgraph_width=tgraph.width
		tgraph_height=tgraph.height

		nrpoints=len(self.datapoints["xmin"])

		for i in range(nrpoints):

			xmi=self.datapoints["xmin"][i]
			xma=self.datapoints["xmax"][i]
			ymi=self.datapoints["ymin"][i]
			yma=self.datapoints["ymax"][i]
			pvalue=self.datapoints["zval"][i]

			xa,ya=tgraph.pos(xmi,ymi)
			xb,yb=tgraph.pos(xma,yma)

			xa=int(round((xa-tgraph_xpos)/tgraph_width*xsize))
			xb=int(round((xb-tgraph_xpos)/tgraph_width*xsize))
			ya=ysize-int(round((ya-tgraph_ypos)/tgraph_height*ysize))
			yb=ysize-int(round((yb-tgraph_ypos)/tgraph_height*ysize))
																																				
			if pvalue==None:
				cvalue=-1.0
			else:				
				if not self.logscal:
					cvalue=(pvalue-self.minval)/(self.maxval-self.minval)
				else:
					try:
						cvalue=(math.log10(pvalue)-math.log10(self.minval))/ \
							(math.log10(self.maxval)-math.log10(self.minval))
					except:
						cvalue=-1.0
												
			if cvalue >=0.0 and cvalue <=1.0:
				col=self.gradient.getcolor(cvalue).rgb().color
				col_str=chr(int(255.0*col["r"]))+chr(int(255.0*col["g"]))+chr(int(255.0*col["b"]))
			else:
				col_str=3*chr(255)

			for ix in range(min(xa,xb),max(xa,xb)):
				for iy in range(min(ya,yb),max(ya,yb)):
					if ix>-1 and ix<xsize and iy>-1 and iy<ysize:
						k=iy*xsize+ix
						pic[k]=col_str

		picstr=""
		for pixel in pic:
			picstr+=pixel

		image_rgb=bitmap.image(xsize,ysize,"RGB",picstr)
		bitmap_rgb=bitmap.bitmap(tgraph.xpos,tgraph.ypos,image_rgb,
			height=tgraph.height,width=tgraph.width)

		privatedata.rectcanvas.insert(bitmap_rgb)

	def adjustaxis(self, privatedata, sharedata, graph, plotitem, columnname, data):
		self.datapoints[columnname]=data
		if columnname=="zval":
			if self.minval==None:
				self.minval=min(x for x in data if x is not None)												
			if self.maxval==None:
				self.maxval=max(x for x in data if x is not None)		

	def drawpoint(self, privatedata, sharedata, graph, point):
		pass

	def key_pt(self, privatedata, sharedata, graph, x_pt, y_pt, width_pt, height_pt):
		raise RuntimeError("Style currently doesn't provide a graph key")

