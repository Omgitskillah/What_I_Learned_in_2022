import os
from instagrapi import Client
from re import search
import requests
import sys
import time
import traceback
from PIL import Image

cookies_file_name = "cookies.json"
username = ""
password = ""
verification = ""

username = "YourInstagramhandle"
password = "YourInstagramPassowd"
verification = "2FACode"

google_script_url = "https://script.google.com/macros/s/GoogleSheetUniqueID/exec"
get_showing_params = {'command':'get_showing'}
set_to_posted_params = {'command':'set_to_posted', 'row':0}


def get_showing_lesson_row():
    row = 0
    status = 0
    try:
        results = requests.get(url = google_script_url, params = get_showing_params)
        status = results.status_code
        if status == 200:
            row = int(results.content)
    except Exception as e:
        print("could not reach the Google App script")
        print(e)
        status = 0
        row = 0
    return row
    
def update_lesson_as_posted(row):
    status = 0
    post_success = False
    try:
        set_to_posted_params['row'] = row
        results = requests.get(url = google_script_url, params = set_to_posted_params)
        status = results.status_code
        if status == 200:
            print("Lesson Row Updated")
            post_success = True
    except Exception as e:
        print("could not reach the Google App script")
        print(e)
    return post_success

def take_photo(pic_name):
    os.system("gphoto2 --capture-image-and-download")
    image = Image.open(r"capt0000.jpg")
    image.thumbnail((1200, 1200))
    image.save(pic_name)
    os.remove(r"capt0000.jpg")
    if True == os.path.exists(pic_name):
        return True
    else:
        return False

def check_for_cookies():
    return os.path.exists(cookies_file_name)

def login_to_instagram(ig, uname, pwd, code):
    try:
        if True == check_for_cookies():
            print("Using cookies found in system")
            ig.load_settings(cookies_file_name)
            ig.login(uname, pwd, verification_code = code)
        else:
            print("No cookies found")
            ig.login(uname, pwd, verification_code = code)
            ig.dump_settings(cookies_file_name)
        
    except Exception as e:
        print("login failed with exception: ")
        print(e)
        traceback.print_exc()
    else:
        print("login success")

def check_if_logged_in( ig ):
    user_id = ig.user_id
    if None == user_id:
        print("No User ID found")
        return False
    else:
        print("user ID: " + str(ig.user_id));
        return True

def post_picture_story( ig, picture_url):
    if ig != None:
        try:
            print("Photo to upload: " + picture_url)
            corrected_path = "./" + picture_url
            print("Corrected Photo to upload: " + corrected_path)
            ig.photo_upload_to_story(corrected_path)
        except:
            print("story upload failed")
            traceback.print_exc()
            return False
        else:
            print("story uploaded successfuly")
            return True

def process_command():
    new_command_prefix = "take_photo:"
    new_command = command.readline()
    if search("take_photo:", str(new_command)):
        return new_command[len(new_command_prefix):]
    else:
        return None

def respond_to_command():
    command.write(unicode("success\n"))

#1. log in
#2. check if logged in
#3. check for any showing lessons
#4. if there is a lesson showing
#5. take a picture then resize it to met instagram requirements
#6. post picture to instagram
#7. if post was successful
#8. set showing image as posted
#9. wait 15 mins then start from 2
heartbeats = 0;
new_ig = Client()
login_to_instagram(new_ig, username, password, verification)

if False == check_if_logged_in(new_ig):
    sys.exit()

while True:
    showing_row = get_showing_lesson_row()
    if showing_row != 0:
        picture_name = "lesson" + str(showing_row) + ".jpg"
        if True == take_photo(picture_name):
            if True == post_picture_story(new_ig, picture_name):
                if False == update_lesson_as_posted(showing_row):
                    new_ig.logout()
                    sys.exit()
            else:
                new_ig.logout()
                sys.exit()
        else:
            new_ig.logout()
            sys.exit()
    print("heartbeat:................................... " + str(heartbeats))
    heartbeats += 1
    time.sleep(60) #wait 1 mins
        
            
        
