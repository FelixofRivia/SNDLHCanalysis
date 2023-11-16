import os
import time
import sys
import argparse

# ANSI escape codes for text colors
class Colors:
    RESET = '\033[0m'
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    PURPLE = '\033[95m'
    CYAN = '\033[96m'
    WHITE = '\033[97m'

def format_seconds(seconds):
    # Calculate hours, minutes, and remaining seconds
    hours, remainder = divmod(seconds, 3600)
    minutes, seconds = divmod(remainder, 60)

    # Create a formatted time string
    time_string = "{:02}h {:02}min {:02}s".format(int(hours), int(minutes), int(seconds))

    return time_string

def check_file_modification(file_path, max_window, is_stopped):
    try:
        text = ""
        # Get the last modification time of the file
        last_modified_time = os.path.getmtime(file_path)

        # Get the current time
        current_time = time.time()
        curr_date = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(current_time))
        last_date = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(last_modified_time))

        last_mod = current_time - last_modified_time

        # Check if the file has been modified within the last 10 minutes
        if last_mod < max_window*60:
            print(" " * len(text), end="\r")
            text = f"{curr_date}: The file {file_path} has been modified within the last {max_window} minutes." 
            print(text, end="\r")
            return True
        else:
            print(" " * len(text), end="\r")
            text = f"{curr_date}: [WARNING] The file {file_path} last update was {format_seconds(last_mod)} ago ({last_date})."
            print(text, end="\r")
            if (last_mod>2*3600):
                sys.exit()  
            return False

    except FileNotFoundError:
        print(f"File not found: {file_path}")
        return False
    except Exception as e:
        print(f"An error occurred: {e}")
        return False

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('brick', type=int)
    parser.add_argument('plate', type=int)
    args = parser.parse_args()
    brick = args.brick
    plate = args.plate
    print(brick)
    print(plate)
    
    file_path = "Desktop/palline.txt"  # Replace with the path to your file
    max_window = 1 #max minutes before sending warning
    is_stopped = False
    while (True):
        is_stopped = check_file_modification(file_path, max_window, is_stopped)
        time.sleep(10)