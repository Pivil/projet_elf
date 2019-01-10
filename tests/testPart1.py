#!/usr/bin/python3
import os

Bleu="\033[1;36m"
Red="\033[0;31m "
Green="\033[0;32m"
White="\033[1;37m"



#1: The path to the progElf
#2: The path to the examples
fileOption = open("optionPart1.txt","r")
content = fileOption.read()
option = content.split()
pathProg = option[0]
pathExamples = option[1]
fileOption.close()



for f in os.listdir(pathExamples): #For all the files in the directory
    pathFileTest = pathExamples+f

    if os.path.isfile(pathFileTest) and str.find(f,".o")!=-1: #For all .o

        print(Bleu+f+White) #Print the name of the file

################################################################################
                                    #Test step 1#
################################################################################

        os.system("arm-eabi-readelf -h "+pathFileTest+" > fileTmp.txt") # Run readelf
        fileCorrection = open("fileTmp.txt","r") # Open the file which contains the content of readelf

        os.system(pathProg+" -h "+pathFileTest+" > fileTmp2.txt") #Run our readelf
        fileToTest = open("fileTmp2.txt","r") # Open the file which contains the content of our function readelf

        contentCorrection = fileCorrection.read()
        tabCorrection = contentCorrection.split("\n")
        contentToTest = fileToTest.read()
        tabToTest= contentToTest.split("\n")


        isCorrect = 1
        error = ""
        for i in range(1,3):
            if str.strip(tabCorrection[i].split(':')[-1]) != str.strip(tabToTest[i].split(':')[-1]):
                isCorrect=0
                error += tabCorrection[i].split(':')[-1] +" | " + tabToTest[i].split(':')[-1]+"\n"

        if tabCorrection[3].split(',')[-1] != tabToTest[3].split(':')[-1]:
            isCorrect=0
            error += tabCorrection[3].split(':')[-1] +" | " + tabToTest[3].split(':')[-1]+"\n"

        if tabCorrection[4].split(' ')[-2] != tabToTest[4].split(' ')[-1]:
            isCorrect=0
            error += tabCorrection[4].split(' ')[-2] +" | "+ tabToTest[4].split(' ')[-1]+"\n"

        for i in range(5,len(tabCorrection)):
            if i==13: #For the line Flags
                if str.strip(tabCorrection[i].split(':')[-1].split(',')[0]) != str.strip(tabToTest[i].split(':')[-1]):
                    isCorrect=0
                    error += tabCorrection[i].split(':')[-1] +" | " + tabToTest[i].split(':')[-1]+"\n"
            else:
                if str.strip(tabCorrection[i].split(':')[-1]) != str.strip(tabToTest[i].split(':')[-1]):
                    isCorrect=0
                    error += tabCorrection[i].split(':')[-1] +" | " + tabToTest[i].split(':')[-1]+"\n"




        if isCorrect:
            print("Elf Header: " + Green+" OK"+White)
        else:
            print("Elf Header: " + Red+" Erreur"+White)
            print(error)

        fileCorrection.close()
        fileToTest.close()


################################################################################
                                    #Test step 2 : Section table#
################################################################################

        os.system("arm-eabi-readelf -S "+pathFileTest+" > fileTmp.txt")
        os.system(pathProg+" -S "+pathFileTest+" > fileTmp2.txt")
        fileCorrection = open("fileTmp.txt","r") # Open the file which contains the content of readelf
        fileToTest = open("fileTmp2.txt","r") # Open the file which contains the content of our function readelf
        contentCorrection = fileCorrection.read()
        tabSectionCorrection = contentCorrection.split("\n")
        contentToTest = fileToTest.read()
        tabSectionToTest= contentToTest.split("\n")

        isCorrect = 1
        error = ""
        for l in range(1,min(len(tabSectionToTest)-1,len(tabSectionCorrection))): #Read all the line of the table
            if str.strip(tabSectionCorrection[l+3]) != str.strip(tabSectionToTest[l]):
                isCorrect=0
                error += str.strip(tabSectionCorrection[l+3]) +"\n" + tabSectionToTest[l]+"\n"

        if isCorrect:
            print("Section Table: " + Green+" OK"+White)
        else:
            print("Section Table: " + Red+" Erreur"+White)
            print(error)

        fileCorrection.close()
        fileToTest.close()

################################################################################
                                    #Test step 3 Section content#
################################################################################
        error=""
        if isCorrect: #If the table of section is correct

            #Use the index
            for l in range(2,len(tabSectionToTest)-3): #For all the section in the section table
                os.system("arm-eabi-readelf -x "+str(l)+" "+pathFileTest+" > fileTmp.txt") #Execute the program with the index l
                os.system(pathProg+" -x "+str(l)+" "+pathFileTest+" > fileTmp2.txt")
                fileCorrection = open("fileTmp.txt","r") # Open the file which contains the content of readelf
                fileToTest = open("fileTmp2.txt","r") # Open the file which contains the content of our function readelf
                tabCorrection = fileCorrection.readlines()
                tabToTest = fileToTest.readlines()

                #The correction has the line "NOTE: This section has relocations against it, but these have NOT been applied to this dump."
                dec = 1 # The offset
                if len(tabCorrection)>=3 and str.find(tabCorrection[2],"NOTE:")!=-1:
                    dec=2

                #If the section is empty
                if not str.find(tabToTest[0],"empty") and str.find(tabCorrection[1],"no data") or str.find(tabToTest[0],"empty") and not str.find(tabCorrection[1],"no data"):
                    isCorrect=0
                    error += str.strip(tabCorrection[1]) +"\n" + tabToTest[0]+"\n"

                #Section not empty
                else:
                    for i in range(1,len(tabToTest)-1):
                        if str.strip(tabCorrection[i+dec]) != str.strip(tabToTest[i]):
                            isCorrect=0
                            error += "Section "+str(l)+":  "+str.strip(tabCorrection[i+dec]) +"\n" + tabToTest[i] +"\n"

            if isCorrect:
                print("Section content by Index: " + Green+" OK"+White)
            else:
                print("Section content by Index: " + Red+" Erreur"+White)
                print(error)

            #Use the name
            isCorrect = 1
            nameSection = ""
            for l in range(2,len(tabSectionToTest)-3): #For all the section in the section table
                # Get the name of the section
                nameSection = tabSectionToTest[l].split("]")[1].split(" ")[1]
                #If the length of the name is 17 it means that the name is toncated
                if (len(nameSection)<17):
                    os.system("arm-eabi-readelf -x "+nameSection+" "+pathFileTest+" > fileTmp.txt")
                    os.system(pathProg+" -x "+nameSection+" "+pathFileTest+" > fileTmp2.txt")
                    fileCorrection = open("fileTmp.txt","r") # Open the file which contains the content of readelf
                    fileToTest = open("fileTmp2.txt","r") # Open the file which contains the content of our function readelf

                    tabCorrection = fileCorrection.readlines()
                    tabToTest = fileToTest.readlines()
                    #The correction has the line "NOTE: This section has relocations against it, but these have NOT been applied to this dump."
                    dec = 1 # The offset
                    if len(tabCorrection)>=3 and str.find(tabCorrection[2],"NOTE:")!=-1:
                        dec=2

                    #If the section is empty
                    if not str.find(tabToTest[0],"empty") and str.find(tabCorrection[1],"no data") or str.find(tabToTest[0],"empty") and not str.find(tabCorrection[1],"no data"):
                        isCorrect=0
                        error += str.strip(tabCorrection[1]) +"\n" + tabToTest[0]+"\n"

                    #Section not empty
                    else:
                        for i in range(1,len(tabToTest)-1):
                            if str.strip(tabCorrection[i+dec]) != str.strip(tabToTest[i]):
                                isCorrect=0
                                error += "Section "+nameSection+":  "+str.strip(tabCorrection[i+dec]) +"\n" + tabToTest[i] +"\n"

            if isCorrect:
                print("Section content by Name: " + Green+" OK"+White)
            else:
                print("Section content by Name: " + Red+" Erreur"+White)
                print(error)

################################################################################
                                    #Test step 4#
################################################################################
        os.system("arm-eabi-readelf -s "+pathFileTest+" > fileTmp.txt")
        os.system(pathProg+" -s "+pathFileTest+" > fileTmp2.txt")
        fileCorrection = open("fileTmp.txt","r") # Open the file which contains the content of readelf
        fileToTest = open("fileTmp2.txt","r") # Open the file which contains the content of our function readelf
        contentCorrection = fileCorrection.read()
        tabCorrection = contentCorrection.split("\n")
        contentToTest = fileToTest.read()
        tabToTest= contentToTest.split("\n")

        isCorrect = 1
        error = ""
        for l in range(1,min(len(tabToTest)-1,len(tabCorrection))): #Read all the line of the table
                if str.strip(tabCorrection[l+2]) != str.strip(tabToTest[l]):
                    isCorrect=0
                    error += str.strip(tabCorrection[l+2]) +"\n" + str.strip(tabToTest[l])+"\n"

        if isCorrect:
            print("Symbol table: "+ Green+" OK"+White)
        else:
            print("Symbol table: " + Red+" Erreur"+White)
            print(error)

        fileCorrection.close()
        fileToTest.close()

################################################################################
                                    #Test step 5#
################################################################################
        os.system("arm-eabi-readelf -r "+pathFileTest+" > fileTmp.txt")
        os.system(pathProg+" -r "+pathFileTest+" > fileTmp2.txt")
        fileCorrection = open("fileTmp.txt","r") # Open the file which contains the content of readelf
        fileToTest = open("fileTmp2.txt","r") # Open the file which contains the content of our function readelf
        contentCorrection = fileCorrection.read()
        tabCorrection = contentCorrection.split("\n")
        contentToTest = fileToTest.read()
        tabToTest= contentToTest.split("\n")

        isCorrect = 1
        error = ""
        for l in range(1,min(len(tabToTest),len(tabCorrection))): #Read all the line of the table
            if str.strip(tabCorrection[l]) != str.strip(tabToTest[l]):
                isCorrect=0
                error += str.strip(tabCorrection[l]) +"\n" + str.strip(tabToTest[l]) + "\n"

        if isCorrect:
            print("Relocation table: " + Green+" OK"+White)
        else:
            print("Relocation table: " + Red+" Erreur"+White)
            print(error)

        fileCorrection.close()
        fileToTest.close()
        print("------------------------------------------------------------")
