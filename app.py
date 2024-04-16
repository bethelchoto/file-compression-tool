import os
import shutil
import time
import glob
from flask import Flask, jsonify, redirect, render_template, request, send_file, send_from_directory, abort
from werkzeug.exceptions import RequestTimeout, HTTPException, NotFound

# Configure Application
app = Flask(__name__)

timeout_seconds = 120

global filename
global ftype

'''
ERROR HANDLING ROUTES
'''
@app.before_request
def before_request():
    request.start_time = time.time()

@app.after_request
def after_request(response):
    elapsed_time = time.time() - request.start_time
    if elapsed_time > timeout_seconds:
        redirect('/timeout_error')
    return response

# 
@app.route("/timeout_error")
def handle_other_exception(e):
    error = "Connection Timed Out, there is probably an issue\n" + e
    return render_template("error_handling.html", error=error), 408

@app.errorhandler(RequestTimeout)
def handle_request_timeout_error(error):
    return render_template('error_handling.html', error=error)

@app.errorhandler(HTTPException)
def handle_request_timeout_error(error):
    return render_template('error_handling.html', error=error)

@app.errorhandler(NotFound)
def handle_request_timeout_error(error):
    return render_template('error_handling.html', error=error)

@app.errorhandler(500)
def handle_other_exception(e):
    return render_template("error_handling.html", error=e), 500

@app.errorhandler(Exception)
def handle_exception(e):
    return render_template("error_handling.html", error=e), 500


'''
WEB ROUTES
'''

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

current_directory = os.path.dirname(os.path.abspath(__file__))

file_upload_directory = os.path.join(current_directory, "temp")

app.config["FILE_UPLOADS_TEMP"] = file_upload_directory

file_upload_directory = os.path.join(current_directory, "uploads")

app.config["FILE_UPLOADS"] = file_upload_directory

app.config["DOWNLOAD_FOLDER"] = "downloads/"

@app.route("/compresszip", methods=["GET", "POST"])
def compresszip():
    if request.method == "GET":
        return render_template("compress.html", action="/compresszip", check=0)
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
                    file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], '{}{}'.format(filename, ftype))
                    file_size = os.path.getsize(file_path)
                    return render_template("compress.html", action="/compresszip", check=1, filename=filename, ftype=ftype, file_size=file_size)
        else:
            print("ERROR")
            return render_template("index.html", check=-1)
        

@app.route("/compressimage", methods=["GET", "POST"])
def compressimage():
    if request.method == "GET":
        return render_template("compress.html", action="/compressimage", check=0)
    else:
        up_file = request.files["file"]
        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('./executables/img_compress uploads/{}'.format(filename))           
            filename = filename[:filename.index(".",1)]
            ftype = "_compressed.bin"
            while True:
                if 'uploads/{}_compressed.bin'.format(filename) in glob.glob('uploads/*_compressed.bin'):
                    os.system('mv uploads/{}_compressed.bin downloads/'.format(filename))
                    file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], '{}{}'.format(filename, ftype))
                    file_size = os.path.getsize(file_path)
                    return render_template("compress.html", action="/compressimage", check=1, filename=filename, ftype=ftype, file_size=file_size)

        else:
            print("ERROR")
            return render_template("index.html", check=-1)


@app.route("/compressdoc", methods=["GET", "POST"])
def compressdoc():
    if request.method == "GET":
        return render_template("compress.html", action="/compressdoc", check=0)
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
                    file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], '{}{}'.format(filename, ftype))
                    file_size = os.path.getsize(file_path)
                    return render_template("compress.html", action="/compressdoc", check=1, filename=filename, ftype=ftype, file_size=file_size)
        else:
            print("ERROR")
            return render_template("index.html", check=-1)
  
@app.route("/compresstiff", methods=["GET", "POST"])
def compresstiff():
    if request.method == "GET":
        return render_template("compress.html", action="/compresstiff", check=0)
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
                    file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], '{}{}'.format(filename, ftype))
                    file_size = os.path.getsize(file_path)
                    return render_template("compress.html", action="/compresstiff", check=1, filename=filename, ftype=ftype, file_size=file_size)
        else:
            print("ERROR")
            return render_template("index.html", check=-1)
        

@app.route("/download/<filename>")
def download_file(filename):
    file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], filename)
    file_size = os.path.getsize(file_path)
    return send_from_directory(app.config["DOWNLOAD_FOLDER"], filename, as_attachment=True)     

@app.route("/index2")
@app.route("/decompress")
def decompress():
    return render_template("de-index.html")

@app.route("/decompressimg", methods=["GET", "POST"])
def decompressimg():
    if request.method == "GET":
        return render_template("decompress.html", action="/decompressimg", check=0)
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
                    file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], '{}{}'.format(filename, ftype))
                    file_size = os.path.getsize(file_path)
                    return render_template("decompress.html", action="/decompressimg", check=1, filename=filename, ftype=ftype, file_size=file_size)
        else:
            print("ERROR")
            return render_template("de-index.html", check=-1)
        
@app.route("/decompressfile", methods=["GET", "POST"])
def decompressfile():
    if request.method == "GET":
        return render_template("decompress.html", action="/decompressfile", check=0)
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
                    file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], '{}{}'.format(filename, ftype))
                    file_size = os.path.getsize(file_path)
                    return render_template("decompress.html", action="/decompressfile", check=1, filename=filename, ftype=ftype, file_size=file_size)
        else:
            print("ERROR")
            return render_template("de-index.html", check=-1)
        

@app.route("/decompresszip", methods=["GET", "POST"])
def decompressedzip():
    if request.method == "GET":
        return render_template("decompress.html", action="/decompresszip", check=0)
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
            filename_ = filename.split("/")[1]
            filename = filename_.split(".")[0]
            ftype = "." + filename_.split(".")[1]
            file_path = os.path.join(app.config["DOWNLOAD_FOLDER"], '{}{}'.format(filename, ftype))
            file_size = os.path.getsize(file_path)
            return render_template("decompress.html", action="/decompresszip", check=1, filename=filename, ftype=ftype, file_size=file_size)
        else:
            print("ERROR")
            return render_template("decompress.html", check=-1)

# Run the Flask application
if __name__ == "__main__":
    app.run(debug=True)
