import os
import shutil
import time
import glob
from flask import Flask, redirect, render_template, request, send_file, send_from_directory

# Configure Application
app = Flask(__name__)

global filename
global ftype

@app.route("/")
@app.route("/home")
def home():
        # Delete old files
    filelist = glob.glob('uploads/*')
    for f in filelist:
        os.remove(f)
    filelist = glob.glob('downloads/*')
    for f in filelist:
        os.remove(f)
    filelist = glob.glob('temp/*')
    for f in filelist:
        os.remove(f)
    return render_template("index.html")

@app.route("/compressimage")
def compress_image():
    return render_template("compress-image.html")

@app.route("/compresszip")
def compress_zip():
    return render_template("compress-zip.html")

@app.route("/compresstiff")
def compress_tiff():
    return render_template("compress-tiff.html")

@app.route("/compressdoc")
def compress_doc():
    return render_template("compress-doc.html")

@app.route("/decompresszip")
def decompresszip():
    return render_template("decompresszip.html")

current_directory = os.path.dirname(os.path.abspath(__file__))

file_upload_directory = os.path.join(current_directory, "temp")

app.config["FILE_UPLOADS_TEMP"] = file_upload_directory

@app.route("/compresszip", methods=["GET", "POST"])
def compresszip():
    if request.method == "GET":
        return render_template("compress-zip.html", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS_TEMP"], filename))
            os.system('./executables/zipper compress temp zipped/zipped.gz')            
            filename = "zipped"
            ftype = ".gz"
            while True:
                if 'zipped/zipped.gz' in glob.glob('zipped/*.gz'):
                    os.system('mv zipped/zipped.gz downloads/')
                    break
            
            return render_template("compress-zip.html", check=1, filename=filename, ftype=ftype)
        else:
            print("ERROR")
            return render_template("index.html", check=-1)
        
file_upload_directory = os.path.join(current_directory, "uploads")

app.config["FILE_UPLOADS"] = file_upload_directory

@app.route("/compressimage", methods=["GET", "POST"])
def compressimage():
    if request.method == "GET":
        return render_template("compress-image.html", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('./executables/img_compress uploads/{}'.format(filename))            
            filename = filename[:filename.index(".",1)]
            ftype = "_compressed.bin"
            while True:
                if 'uploads/{}_compressed.bin'.format(filename) in glob.glob('uploads/*_compressed.bin'):
                    os.system('mv uploads/{}_compressed.bin downloads/'.format(filename))
                    break
           
            return render_template("compress-image.html", check=1, filename=filename, ftype=ftype)

        else:
            print("ERROR")
            return render_template("index.html", check=-1)


@app.route("/compressdoc", methods=["GET", "POST"])
def compressdoc():
    if request.method == "GET":
        return render_template("compress-doc.html", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('./executables/doc_compress uploads/{}'.format(filename))            
            filename = filename[:filename.index(".",1)]
            ftype = "_compressed.bin"

            while True:
                if 'uploads/{}_compressed.bin'.format(filename) in glob.glob('uploads/*_compressed.bin'):
                    os.system('mv uploads/{}_compressed.bin downloads/'.format(filename))
                    break
            return render_template("compress-doc.html", check=1, filename=filename, ftype=ftype)
        else:
            print("ERROR")
            return render_template("index.html", check=-1)
  
@app.route("/compresstiff", methods=["GET", "POST"])
def compresstiff():
    if request.method == "GET":
        return render_template("compress-tiff.html", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('./executables/tiff_compress uploads/{}'.format(filename))            
            filename = filename[:filename.index(".",1)]
            ftype = "_compressed.bin"
            while True:
                if 'uploads/{}_compressed.bin'.format(filename) in glob.glob('uploads/*_compressed.bin'):
                    os.system('mv uploads/{}_compressed.bin downloads/'.format(filename))
                    break
            return render_template("compress-tiff.html", check=1, filename=filename, ftype=ftype)
        else:
            print("ERROR")
            return render_template("index.html", check=-1)
        
app.config["DOWNLOAD_FOLDER"] = "downloads/"
@app.route("/download/<filename>")
def download_file(filename):
    return send_from_directory(app.config["DOWNLOAD_FOLDER"], filename, as_attachment=True)     

@app.route("/index2")
@app.route("/decompress")
def decomp():
    return render_template("de-index.html")

@app.route("/decompressimg", methods=["GET", "POST"])
def decompress():
    if request.method == "GET":
        return render_template("decompress.html", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('./executables/img_decompress uploads/{}'.format(filename))
            f = open('uploads/{}'.format(filename), 'rb')
            ftype = "_decompressed." + "jpg" # make this user choice btwn png/jpg
            filename = filename[:filename.index("_compressed",1)]

            while True:
                if 'uploads/{}{}'.format(filename, ftype) in glob.glob('uploads/*_decompressed.*'):
                    os.system('mv uploads/{}{} downloads/'.format(filename, ftype))
                    break
            return render_template("decompress.html", check=1, filename=filename, ftype=ftype)
        else:
            print("ERROR")
            return render_template("decompress.html", check=-1)
        
@app.route("/decompressfile", methods=["GET", "POST"])
def decompressfile():
    if request.method == "GET":
        return render_template("decompressfile.html", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('./executables/doc_decompress uploads/{}'.format(filename))
            f = open('uploads/{}'.format(filename), 'rb')
            ftype = "_decompressed." + (f.read(int(f.read(1)))).decode("utf-8")
            filename = filename[:filename.index("_compressed",1)]

            while True:
                if 'uploads/{}{}'.format(filename, ftype) in glob.glob('uploads/*_decompressed.*'):
                    os.system('mv uploads/{}{} downloads/'.format(filename, ftype))
                    break
            # return render_template("decompress.html", check=1)
            return render_template("decompressfile.html", check=1, filename=filename, ftype=ftype)
        else:
            print("ERROR")
            return render_template("decompressfile.html", check=-1)
        

@app.route("/decompressedzip", methods=["GET", "POST"])
def decompressedzip():
    if request.method == "GET":
        return render_template("decompresszip.html", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            # up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))

            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('./executables/zipper decompress uploads/zipped.gz downloads'.format(filename))  

            filename = glob.glob(os.path.join('downloads/', "file_0*"))[0]
            filename = filename.split("/")[1]
            print(filename)

            return render_template("decompresszip.html", check=1, filename=filename)
        else:
            print("ERROR")
            return render_template("decompress.html", check=-1)

# Run the Flask application
if __name__ == "__main__":
    app.run(debug=True)
