import os,glob,string

#def Test1(rootDir):
#    list_dirs = os.walk(rootDir)
#    for root, dirs, files in list_dirs:
#        for d in dirs:
#            print os.path.join(root, d)
#        for f in files:
#           print os.path.join(root, f)

opTypeDict = {}
playerDict = {}
outLogFile = "AuditItem-28.log"
opTypeLogFile = "AuditByOpType-28.log"
playerTotalLogFile = "AuditPlayer-28.log"

def parseop(line):
        slit = line.split('\1')
        keyDict = ""
        num = 0
        playerGuid = ""
        for item in slit:
                item.strip()
                idx = item.find("=")
                if idx >= 0:
                        key=item[:idx]
                        val=item[idx+1:]
                        if key.find("OPType") >= 0:
                                keyDict =  key + "\t" + val
                        if key.find("OPNum") >= 0:
                                num = string.atoi(val)

                elif item.find(",") >= 0:
                        playerGuid = item.replace(",", "#")
                        
        if cmp(keyDict, "") != 0:
                if keyDict in opTypeDict:
                        opTypeDict[keyDict] += num
                else:
                        opTypeDict[keyDict] = num
                        
                if playerGuid in playerDict:
                        playerDict[playerGuid] += 1
                else:
                        playerDict[playerGuid] = 1
                        

def parseFile(path,log):
    fo = open(path+os.sep+ ".." + os.sep + ".." + os.sep + outLogFile, "a+")
    fi = open(log, "r")
    for line in fi:
        p = line.rfind("DataId=28 ")#28
        if p >= 0:
                fo.write(line)
                parseop(line)
    fi.close()

def parseDir( path ):
    for fn in glob.glob( path + os.sep + '*' ): # '*'代表匹配所有文件
        if os.path.isdir( fn ): # 如果结果为文件夹
            parseDir( fn ) # 递归
        else:
                if fn.find(".log") > 0:
                        print "Parsing ",fn
                        parseFile(path, fn)

def writeOutLog(dictp, path, fn):
        fo = open(path+os.sep + ".." + os.sep+ fn, "a+")
        for key in dictp:
                text = key + "\t" + str(dictp[key]) + "\n"
                fo.write(text)
        fo.close()

logPath = r"E:\Log"
parseDir(logPath)
print "Done!"

writeOutLog(opTypeDict, logPath, opTypeLogFile)
writeOutLog(playerDict, logPath, playerTotalLogFile)
