#************************************************
#Title      :DesignRegionMakerFromVTK.py
#Author     :Tanabe Yuta
#Date       :2019/10/23
#Copyright  :(C)2019 TanabeYuta
#************************************************


#************************************************
#   VTKModel class
#************************************************
class VTKModel(object):
    #**********Made by VTK file**********
    def __init__(self, _fname = None):
        self.fname = ""         #file name
        self.points = []        #points corrdinate value
        self.cells = []         #points id in cell
        self.celltypes = []     #cell types
        self.pointscaler = {}   #point scaler datas
        self.pointvector = {}   #point vector datas
        self.cellscaler = {}    #point scaler datas
        self.cellvector = {}    #point vector datas

        if(not _fname == None):
            self.fname = _fname     #file name

            with open(_fname) as fp:
                mode = 0     #  0:POINT_DATA    1:CELL_DATA
                for line in fp:
                    linesplit =  line.split()
                    if(len(linesplit) != 0):
                        #----------Get POINTS----------
                        if(linesplit[0] == "POINTS"):
                            for i in range(int(linesplit[1])):
                                self.points.append(list(map(float, fp.readline().split())))
                        
                        #----------Get CELLS----------
                        if(linesplit[0] == "CELLS"):
                            for i in range(int(linesplit[1])):
                                self.cells.append(list(map(int, fp.readline().split()[1:])))
                        
                        #----------Get CELL_TYPES----------
                        if(linesplit[0] == "CELL_TYPES"):
                            for i in range(int(linesplit[1])):
                                self.celltypes.append(int(fp.readline()))
                    
                        #----------Set mode as POINT_DATA----------
                        if(linesplit[0] == "POINT_DATA"):
                            mode = 0

                        #----------Set mode as CELL_DATA----------
                        if(linesplit[0] == "CELL_DATA"):
                            mode = 1
                            
                        #----------Get SCALARS----------
                        if(linesplit[0] == "SCALARS"):
                            dataname = linesplit[1]
                            fp.readline()
                            tmplist = []
                            if(mode == 0):
                                for i in range(len(self.points)):
                                    tmplist.append(float(fp.readline()))
                                self.pointscaler[dataname] = tmplist
                            elif(mode == 1):
                                for i in range(len(self.cells)):
                                    tmplist.append(float(fp.readline()))
                                self.cellscaler[dataname] = tmplist
                        
                        #----------Get VECTORS----------
                        if(linesplit[0] == "VECTORS"):
                            dataname = linesplit[1]
                            tmplist = []
                            if(mode == 0):
                                for i in range(len(self.points)):
                                    tmplist.append(list(map(float, fp.readline().split())))
                                self.pointvector[dataname] = tmplist
                            elif(mode == 1):
                                for i in range(len(self.cells)):
                                    tmplist.append(list(map(float, fp.readline().split())))
                                self.cellvector[dataname] = tmplist


    #**********Output as VTK format**********
    def __str__(self):
        #----------Add Header----------
        retstr = "# vtk DataFile Version 4.1 \n vtk " + self.fname + "\n ASCII \n DATASET UNSTRUCTURED_GRID \n"
        
        #----------Add POINTS----------
        retstr += "\nPOINTS\t" + str(len(self.points)) + "\tfloat\n"
        for point in self.points:
            for x in point:
                retstr += str(x) + "\t"
            retstr += "\n"
        
        #----------Add CELLS----------
        retstr += "\nCELLS\t" + str(len(self.cells)) + "\t" + str(sum(len(cell) + 1 for cell in self.cells)) + "\n"
        for cell in self.cells:
            retstr += str(len(cell)) + "\t"
            for point in cell:
                retstr += str(point) + "\t"
            retstr += "\n"

        #----------Add CELL_TYPES----------
        retstr += "\nCELL_TYPES\t" + str(len(self.cells)) + "\n"
        for celltype in self.celltypes:
            retstr += str(celltype) + "\n"

        #----------Add POINT_DATA----------
        retstr += "\nPOINT_DATA\t" + str(len(self.points)) + "\n"
        for key, values in self.pointscaler.items():
            retstr += "SCALARS\t" + key + "\tfloat\nLOOKUP_TABLE\tdefault\n"
            for value in values:
                retstr += str(value) + "\n"
        for key, values in self.pointvector.items():
            retstr += "VECTORS\t" + key + "\tfloat\n"
            for value in values:
                for u in value:
                    retstr += str(u) + "\t"
                retstr += "\n"
        
        #----------Add CELL_DATA----------
        retstr += "\nCELL_DATA\t" + str(len(self.cells)) + "\n"
        for key, values in self.cellscaler.items():
            retstr += "SCALARS\t" + key + "\tfloat\nLOOKUP_TABLE\tdefault\n"
            for value in values:
                retstr += str(value) + "\n"
        for key, values in self.cellvector.items():
            retstr += "VECTORS\t" + key + "\tfloat\n"
            for value in values:
                for u in value:
                    retstr += str(u) + "\t"
                retstr += "\n"

        return retstr   


#************************************************
#   Filter VTKModel
#************************************************
def FilterVTKModelWithCellScaler(_model, _key, _value):  
    filteredcellid = [i for i in range(len(_model.cellscaler[_key])) if _model.cellscaler[_key][i] >= _value]

    model = VTKModel()
    model.fname = "filtered"
    filteredcells = [_model.cells[cellid] for cellid in filteredcellid]
    model.celltypes = [_model.celltypes[cellid] for cellid in filteredcellid]

    filteredpointid = set()
    for cell in filteredcells:
        filteredpointid = filteredpointid.union(set(cell))
    model.points = [_model.points[pointid] for pointid in filteredpointid]

    modifiedpointid = {}
    for i in range(len(filteredpointid)):
        modifiedpointid[list(filteredpointid)[i]] = i
    
    for cell in filteredcells:
        tmplist = []
        for pointid in cell:
            tmplist.append(modifiedpointid[pointid])
        model.cells.append(tmplist)
    
    for key in _model.pointscaler.keys():
        model.pointscaler[key] = [_model.pointscaler[key][pointid] for pointid in filteredpointid]
    for key in _model.pointvector.keys():
        model.pointvector[key] = [_model.pointvector[key][pointid] for pointid in filteredpointid]

    for key in _model.cellscaler.keys():
        model.cellscaler[key] = [_model.cellscaler[key][pointid] for pointid in filteredcellid]
    for key in _model.cellvector.keys():
        model.cellvector = [_model.cellvector[key][pointid] for pointid in filteredcellid]

    return model
    

#************************************************
#   Export To PANSFEM
#************************************************
def ExportToPANSFEM(_model):
    #----------Export Node.csv----------
    with open(_model.fname + "_node.csv", mode = 'w') as fp:
        retstr = "ID,x0,x1,x2\n"
        for i, point in enumerate(_model.points):
            retstr += str(i)
            for x in point:
                retstr += "," + str(x)
            retstr += "\n"
        fp.write(retstr)
    
    #----------Export Element.csv----------
    with open(_model.fname + "_element.csv", mode = 'w') as fp:
        retstr = "ID,IDP,NON\n"
        for i, points in enumerate(_model.cells):
            retstr += str(i) + "," + str(i) + "," + str(len(points))
            for pointid in points:
                retstr += "," + str(pointid)
            retstr += "\n"
        fp.write(retstr)


#************************************************
#   Main Thread
#************************************************
if __name__ == "__main__":
    fname = "W50.vtk"
    m = FilterVTKModelWithCellScaler(VTKModel(fname), "Rho0", 0.95)
    ExportToPANSFEM(m)