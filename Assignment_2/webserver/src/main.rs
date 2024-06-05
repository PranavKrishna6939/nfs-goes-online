use std::{
    fs,
    io::{prelude::*, BufReader},
    net::{TcpListener, TcpStream},
};

fn main() {
    let listener = TcpListener::bind("127.0.0.1:7878").unwrap();

    for stream in listener.incoming() {
        let stream = stream.unwrap();

        handle_connection(stream);
    }
}

fn handle_connection(mut stream: TcpStream) {
    let buf_reader = BufReader::new(&mut stream);
    
    let request_line = match buf_reader.lines().next() {
        Some(Ok(request_line)) => request_line,
        Some(Err(_)) | None => {
            let error_400 = "HTTP/1.1 400 BAD REQUEST";
            let filename_400 = "400.html";
            let content_400 = fs::read_to_string(filename_400).unwrap();
            let length_400 = content_400.len();

            let response = format!(
                "{error_400}\r\nContent-Length: {length_400}\r\n\r\n{content_400}"
            );
            stream.write_all(response.as_bytes()).unwrap();
            return; 
        }
    };
 

    let (status_line, filename) = if request_line == "GET / HTTP/1.1" {
        ("HTTP/1.1 200 OK", "home.html")
    } else {
        ("HTTP/1.1 404 NOT FOUND", "404.html")
    };
    
    let contents = match fs::read_to_string(filename) {
        Ok(contents) => contents,
        Err(_) => {
            let error_500 = "HTTP/1.1 500 INTERNAL SERVER ERROR";
            let filename_500 = "500.html";
            let content_500 = fs::read_to_string(filename_500).unwrap();
            let length_500 = content_500.len();

            let response = format!(
             "{error_500}\r\nContent-Length: {length_500}\r\n\r\n{content_500}"
            );
            stream.write_all(response.as_bytes()).unwrap();
            return;
        }
    };
      
    let length = contents.len();

    let response =
        format!("{status_line}\r\nContent-Length: {length}\r\n\r\n{contents}");

    stream.write_all(response.as_bytes()).unwrap();
}
