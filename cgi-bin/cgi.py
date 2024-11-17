#!/usr/bin/env python3

import cgi
import time

# Set the content type to be HTML
print("Content-Type: text/html\n")

# Print the HTML header
print("<html>")
print("<head><title>Dynamic CGI Example</title></head>")
print("<body>")

# Display the current server time
current_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
print(f"<p>Current time: {current_time}</p>")

# Display a form for user input
print("""
<form method="GET" action="">
    <label for="name">Enter your name:</label>
    <input type="text" id="name" name="name">
    <input type="submit" value="Submit">
</form>
""")

# Process the user's input
form = cgi.FieldStorage()
name = form.getvalue("name", "Guest")
print(f"<p>Hello, {name}! Welcome to the Python CGI script.</p>")

# Close the HTML
print("</body>")
print("</html>")
