#######################################################
#    Author:        <Matthew Bolda>
#    email:         <mbolda@purdue.edu>
#    ID:            <mbolda>
#    Date Created:  <3/24/2020>
#    Date Updated:  <4/27/2020>
#######################################################

import os
import sys

class Building:
    def __init__(self, abbreviation, fullname, coordinates, building_num, entrances_list):
        self.abbreviation = abbreviation            # the official abbreviation for the building
        self.fullname = fullname                    # the official name of the building
        self.coordinates = coordinates[1:]          # the list of the coordinates for the building
        self.first_coord = coordinates[0]           # the first point of coordinates for the building
        self.building_num = str(building_num)       # the number the building get parsed at
        self.entrance_list = entrances_list         # tuple('letter_ID', 'Accessible Y/N', list of coords)
        self.A_entrance_list = []                   # accessible entrance list
        self.NA_entrance_list = []                  # NONaccessible entrance list

        return

    def printer(self):
        a = '// Creating Building #' + self.building_num + ' ' + self.fullname + '...\n'
        b = 'building = [[Building alloc] initWithShortName:@"' + (
            self.abbreviation).upper() + '" fullName:@"' + self.fullname + '"];\n'
        x1, y1, z1 = self.first_coord
        c = '[building setPathWithPaths:@[@[@' + y1 + ', @' + x1 + '],\n'
        d = ''
        length = len(self.coordinates)
        counter = 0
        for coord in self.coordinates:
            x, y, z = coord
            counter = counter + 1
            if counter == (length):
                d += '                              @[@' + y + ', @' + x + ']]];\n\n'
            else:
                d += '                              @[@' + y + ', @' + x + '],\n'
        e = '[building createPolygonForMapView:mapView];\n\n'
        f = '[buildingsArr addObject:building];\n\n'

        return a + b + c + d + e + f

    def entrance_print(self):

        return

def build_dictionary(KMLfile):
    building_list = []
    in_building = False
    need_name = False
    need_coords = False
    list_of_coords = []
    building_num = 1
    with open(KMLfile, 'r') as f:
        lines = f.readlines()
        for line in lines:
            line = line.strip()

            if line == "<Placemark>":
                in_building = True
                need_name = True
            elif line == "</Placemark>":
                in_building = False
            elif need_name == True:
                name = line
                name = name.strip()
                name = name.strip('<name>')
                name = name.strip('</name>')
                abbreviation = name.split(' ')[0]
                abbreviation = abbreviation.lower()
                name = name.split(' ')
                name = name[1:]
                full_name = ''
                length = len(name)
                index = 0
                for word in name:
                    index += 1
                    full_name += word
                    if index != length:
                        full_name += ' '
                need_name = False
            elif line == "<coordinates>":
                need_coords = True
                list_of_coords = []
            elif line == "</coordinates>":
                need_coords = False
                new_building = Building(abbreviation, full_name, list_of_coords, building_num, [])
                building_num += 1
                building_list.append(new_building)
                #
                # Create the building here
                #
            elif need_coords == True:
                coords = line.strip()
                coords = coords.split(',')
                x = coords[0]
                y = coords[1]
                z = coords[2]
                coordinates = x, y, z
                list_of_coords.append(coordinates)
    return building_list


def build_entrances(KMLfile, building_list):
    entrance_list = []
    in_building = False
    need_name = False
    need_coords = False
    building_num = 1
    with open(KMLfile, 'r') as f:
        lines = f.readlines()
        for line in lines:
            line = line.strip()

            if line == "<Placemark>":
                in_building = True
                need_name = True
            elif line == "</Placemark>":
                in_building = False
            elif need_name == True:
                name = line
                name = name.strip()
                name = name.strip('<name>')
                name = name.strip('</name>')
                abbreviation = name.split(' ')[0]
                abbreviation = abbreviation.lower()
                letter = name.split(' ')[1]
                accessible = name.split(' ')[2]
                full_name = ''
                need_name = False
            elif line == "<coordinates>":
                need_coords = True
                list_of_coords = []
            elif line == "</coordinates>":
                need_coords = False
                for building in list_of_buildings:
                    if building.abbreviation.upper() == abbreviation.upper():
                        building.entrance_list.append((letter, accessible, list_of_coords))
                        if accessible == 'YES':
                            building.A_entrance_list.append((letter, list_of_coords))
                        if accessible == 'NO':
                            building.NA_entrance_list.append((letter, list_of_coords))
                        if accessible != 'NO' and accessible != 'YES':
                            print('There was an erorr, one of the entrances was not specified to be accessible or not')
                            print('Please use exactly "YES" or "NO" to specify')
                            print('The building with a problem is ' + str(building.abbreviation) + 'with entrance '+ str(letter))
                        break
                #
                # Create the building here
                #
            elif need_coords == True:
                coords = line.strip()
                coords = coords.split(',')
                x = coords[0]
                y = coords[1]
                z = coords[2]
                coordinates = x, y, z
                list_of_coords.append(coordinates)
    return

# Usage:
# if 0 parameters, script will assume files are named
# UnParsedBuilding.txt ParsedBuilding.txt UnParsedEntrances.txt ParsedEntrances.txt

# if 2 parameters, script will assume
# first input is KML file for Buildings, second input is KML file for entrances
# outputs will be ParsedBuilding.txt ParsedEntrances.txt

if __name__ == "__main__":

    if len(sys.argv) == 1:
        BuildingFileName = "UnParsedBuildings.txt"  # change to whatever your txt file is named for buildings
        OutputFileName = "ParsedBuildings.txt"  # this is where you will find the code to copy and paste for buildings
        EntranceFileName = "UnParsedEntrances.txt"
    elif len(sys.argv) == 3:
        BuildingFileName = sys.argv[1]
        EntranceFileName = sys.argv[2]
        OutputFileName = "ParsedBuildings.txt"
    else:
        print( '# Usage:\n\
# if 0 parameters, script will assume files are named\n\
# UnParsedBuilding.txt ParsedBuilding.txt UnParsedEntrances.txt ParsedEntrances.txt\n\
\n\
# if 2 parameters, script will assume\n\
# first input is KML file for Buildings, second input is KML file for entrances\n\
# outputs will be ParsedBuilding.txt ParsedEntrances.txt\n')


    if len(sys.argv) == 1 or len(sys.argv) == 3:
        list_of_buildings = build_dictionary(BuildingFileName)

        output_file = open(OutputFileName, 'w')
        build_entrances(EntranceFileName, list_of_buildings)

        ''' # This is a test to see that build_dictionary is good
        for building in list_of_buildings:
            build = building.printer()
            output_file.write(build)
        '''
        #''' # This is a test to see that build_entrances is good
        for building in list_of_buildings:
            if building.abbreviation.upper() == "ARMS":
                #print(building.entrance_list)
                print('All entrances into ' + building.abbreviation.upper())
                print('Letter       Accessible')
                for i in building.entrance_list:
                        print(i[0] + '            '+  i[1])
                #print('Accessible entrances')
                #for i in building.A_entrance_list:
                #    print(i[0])
                #print('Non-Accessible entrances')
                #for i in building.NA_entrance_list:
                #    print(i[0])
        #'''

        output_file.close()
