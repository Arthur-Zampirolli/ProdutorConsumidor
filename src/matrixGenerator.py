import random
TAMANHO = 50
DIMENSION = 10

separators = {
    "comma": ",",
    "pipe": "|",
    "dash": "-",
}

def genMatrixesFile(fileName, sepSymbol):
    out = ''
    file = open(fileName, 'w+')
    for a in range(2):
        for i in range(DIMENSION):
            for j in range(DIMENSION):
                separator=''
                if(j < DIMENSION - 1):
                    separator = sepSymbol
                out+=str(random.random()*10)+separator
            out+='\n'
        if(a < 1):
            out+='\n'
    file.write(out)
    file.close()
def main():
    file = open("input/entrada.in", "w+")
    fileName = "input/set"
    fileNames = ""
    for i in range(TAMANHO):
        fileNames+=fileName+str(i)+".in\n"
    file.write(fileNames)
    file.close()
    split = fileNames.split('\n')
    print(split)
    for i in range(TAMANHO):
        if(len(split[i]) > 0):
            genMatrixesFile(split[i], separators["comma"])
    return 0
main()