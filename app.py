import os
import time
import glob
from flask import Flask, redirect, render_template, request, send_file

# Configure Application
app = Flask(__name__)

global filename
global ftype

@app.route("/")
def home():
    # List of directories containing files to be removed
    directories_uploads = ['uploaddocuments', 'uploadimages', 'uploadtiffimages']

    # Iterate over each directory
    for directory in directories_uploads:
        directory_path = os.path.join(app.root_path, directory)
        if os.path.exists(directory_path):
            for filename in os.listdir(directory_path):
                file_path = os.path.join(directory_path, filename)
                if os.path.isfile(file_path):
                    os.remove(file_path)  # Delete file

    # Remove files from the 'downloads' directory
    downloads_path = os.path.join(app.root_path, 'downloads')
    if os.path.exists(downloads_path):
        for subdir in ['archive', 'Documents', 'images', 'tiff_images']:
            subdir_path = os.path.join(downloads_path, subdir)
            if os.path.exists(subdir_path):
                for filename in os.listdir(subdir_path):
                    file_path = os.path.join(subdir_path, filename)
                    if os.path.isfile(file_path):
                        os.remove(file_path)

    return render_template("index.html")

@app.route("/compress_image")
def compress_image():
    return render_template("compress-image.html")

@app.route("/compress_bin")
def compress_bin():
    return render_template("compress-bin.html")

@app.route("/compress_tiff")
def compress_tiff():
    return render_template("compress-tiff.html")

@app.route("/compress_doc")
def compress_doc():
    return render_template("compress-doc.html")

@app.route("/compress_archive")
def compress_archive():
    return render_template("create-archive.html")


app.config["FILE_UPLOADS"] = "/home/panashe/file-compression-tool/uploadimages"

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

            os.system('./c uploadimages/{}'.format(filename))            
            filename = filename[:filename.index(".",1)]

            ftype = "-compressed.bin"

            while True:
                if 'uploadimages/{}-compressed.bin'.format(filename) in glob.glob('uploadimages/*-compressed.bin'):
                    os.system('mv uploadimages/{}-compressed.bin downloads/images/'.format(filename))
                    break

            return render_template("index.html", check=1)

        else:
            print("ERROR")
            return render_template("index.html", check=-1)

app.config["FILE_UPLOADS_DOC"] = "/home/panashe/file-compression-tool/uploaddocuments"    

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

            up_file.save(os.path.join(app.config["FILE_UPLOADS_DOC"], filename))

            os.system('./c uploaddocuments/{}'.format(filename))            
            filename = filename[:filename.index(".",1)]

            ftype = "-compressed.bin"

            while True:
                if 'uploaddocuments/{}-compressed.bin'.format(filename) in glob.glob('uploaddocuments/*-compressed.bin'):
                    os.system('mv uploaddocuments/{}-compressed.bin downloads/Documents/'.format(filename))
                    break

            return render_template("index.html", check=1)

        else:
            print("ERROR")
            return render_template("index.html", check=-1)

app.config["FILE_UPLOADS_TIFF"] = "/home/panashe/file-compression-tool/uploadtiffimages"    

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

            up_file.save(os.path.join(app.config["FILE_UPLOADS_TIFF"], filename))

            os.system('./tiff uploadtiffimages/{}'.format(filename))            
            filename = filename[:filename.index(".",1)]

            ftype = "-compressed.bin"

            while True:
                if 'uploadtiffimages/{}-compressed.bin'.format(filename) in glob.glob('uploadtiffimages/*-compressed.bin'):
                    os.system('mv uploadtiffimages/{}-compressed.bin downloads/tiff_images/'.format(filename))
                    break
            return render_template("index.html", check=1)

        else:
            print("ERROR")
            return render_template("index.html", check=-1)



app.config["FILE_UPLOADS_BIN"] = "/home/panashe/file-compression-tool/uploadbin"    

@app.route("/compressbin", methods=["GET", "POST"])
def compressbin():
    if request.method == "GET":
        return render_template("compress-bin.html", check=0)
    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)

            up_file.save(os.path.join(app.config["FILE_UPLOADS_BIN"], filename))

            os.system('./rle_bin uploadbin/{}'.format(filename))            
            filename = filename[:filename.index(".",1)]

            ftype = "-compressed.bin"

            while True:
                if 'uploadbin/{}-compressed.bin'.format(filename) in glob.glob('uploadbin/*-compressed.bin'):
                    os.system('mv uploadbin/{}-compressed.bin downloads/bin_images/'.format(filename))
                    break
            return render_template("index.html", check=1)

        else:
            print("ERROR")
            return render_template("index.html", check=-1)


# Run the Flask application
if __name__ == "__main__":
    app.run(debug=True)
