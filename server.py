# Python 3 server example
from http.server import BaseHTTPRequestHandler, HTTPServer
import subprocess

hostName = "192.168.1.10"
serverPort = 8080
info = {}

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):

        result = subprocess.run(['./script.sh'], stdout=subprocess.PIPE)
        result = str(result.stdout).replace('\\n', '\n').replace('\\t', '')[2:-2]#.split('\n')  

        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("<html><head><title>Linux information Arthur :)<title></head>\n", "utf-8"))

        self.wfile.write(bytes("<body>\n", "utf-8"))
        self.wfile.write(bytes("<p></p>\n", "utf-8"))
        for line in result.split('\n'):
            self.wfile.write(bytes(f"<br>  {line}  </br>\n", "utf-8"))
        self.wfile.write(bytes("<p></p>\n", "utf-8"))
        self.wfile.write(bytes("</body></html>", "utf-8"))


if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
