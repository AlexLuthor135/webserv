## Webserv
## This is when you finally understand why a URL starts with HTTP

This project is about writing your own HTTP server.
You are able to test it with an actual browser.
HTTP is one of the most used protocols on the internet.
Knowing its arcane will be useful, and that is the main goal of the project - to master it.

 <details>
<summary>Introduction to HTTP: </summary>
The Hypertext Transfer Protocol (HTTP) is an application protocol for distributed,
collaborative, hypermedia information systems.
HTTP is the foundation of data communication for the World Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access.
For example, by a mouse click or by tapping the screen in a web browser.
HTTP was developed to facilitate hypertext and the World Wide Web.
The primary function of a web server is to store, process, and deliver web pages to
clients. The communication between client and server takes place using the Hypertext
Transfer Protocol (HTTP).
Pages delivered are most frequently HTML documents, which may include images,
style sheets, and scripts in addition to the text content.
Multiple web servers may be used for a high-traffic website.
A user agent, commonly a web browser or web crawler, initiates communication by
requesting a specific resource using HTTP and the server responds with the content of
that resource or an error message if unable to do so. The resource is typically a real file
on the server’s secondary storage, but this is not necessarily the case and depends on how
the webserver is implemented.
While the primary function is to serve content, full implementation of HTTP also
includes ways of receiving content from clients. This feature is used for submitting web
forms, including the uploading of files.
</details>

## Features of the project:

<details>
  <summary>1. Non-Blocking Servers: </summary>
   One of the standout features of our webserv project is its implementation of a non-blocking server architecture using epoll.
   This design ensures efficient and scalable handling of multiple client connections concurrently without the performance bottlenecks 
   commonly associated with traditional blocking servers.

1. **Concurrent Connections**: Non-blocking servers can manage multiple client connections simultaneously. This allows the server to handle a large number of requests at the same time, improving the overall responsiveness and throughput.

2. **Resource Efficiency**: By avoiding the need for dedicated threads or processes for each connection, non-blocking servers use system resources more efficiently. This reduces the overhead associated with context switching and memory usage, leading to better performance.

3. **Reduced Latency**: Non-blocking operations minimize the idle time that typically occurs when waiting for I/O operations to complete. This results in lower latency and faster response times for clients.

4. **Scalability**: As the number of client connections increases, a non-blocking server can scale more effectively than a blocking server. This makes it an ideal choice for applications requiring high concurrency and scalability.

5. **Event-Driven Architecture**: Non-blocking servers utilize an event-driven approach, where the server processes events (such as incoming requests) as they occur. This allows the server to remain responsive and dynamically allocate resources based on current demand.

6. **Improved Reliability**: By handling I/O operations asynchronously, non-blocking servers are less likely to experience bottlenecks or crashes under high load conditions. This enhances the overall reliability and stability of the server.

#### Implementation Details:

- **`epoll` Mechanism**: Our server uses the `epoll` system call, which is highly efficient for monitoring large numbers of file descriptors. `epoll` provides scalable I/O event notification, ensuring that the server can respond to any I/O event as soon as it occurs.
- **Asynchronous I/O**: All I/O operations are performed asynchronously, ensuring that no single request can block the entire server.
- **Event Loop**: The core of the non-blocking server is an event loop that continuously checks for and processes events, maintaining a high level of responsiveness.

By leveraging these principles, our webserv project delivers a robust and high-performance web server capable of handling modern web traffic demands with ease.
  </details>
 </details>
 <details>
   <summary>2. Comprehensive HTTP Method Support</summary>
   Our webserv project includes robust support for multiple HTTP methods, allowing it to handle a wide variety of client requests and perform diverse operations. Here’s a breakdown of the supported methods:

#### **GET Method**
The GET method is used to retrieve data from the server. It is one of the most commonly used HTTP methods. Our server processes GET requests efficiently, ensuring quick and reliable delivery of requested resources.

#### **POST Method**
The POST method is used to send data to the server to create a new resource. It allows clients to submit data such as form entries, file uploads, or other data to the server. Our server ensures that POST requests are handled securely and effectively.

#### **PUT Method**
The PUT method is used to update an existing resource or create a new resource if it does not exist. Our server handles PUT requests by allowing clients to upload or update data, ensuring the correct updating of resources.

#### **DELETE Method**
The DELETE method is used to remove a specified resource from the server. Our server processes DELETE requests efficiently, allowing clients to delete resources as needed while maintaining server integrity and security.

#### **HEAD Method**
The HEAD method is similar to GET but it only retrieves the headers of a resource, without the body. This method is useful for checking what a GET request will return before making the actual request. Our server supports HEAD requests to facilitate quick checks on resources.
 </details>

<details>
  <summary>3. Flexible Configuration Options</summary>
  Our webserv project includes a flexible configuration system that allows users to customize the server’s behavior according to their needs. This includes:

- **Port Configuration**: Users can specify the port on which the server listens for incoming connections.
- **Root Directory**: The server’s root directory can be configured, allowing users to serve files from any specified location on their system.
- **Error Pages**: Custom error pages can be set up for different HTTP error codes, improving the user experience by providing meaningful error messages.
- **Max Body Size**: Users can configure the maximum body size for incoming requests, helping to manage server load and security.
- **Allow methods**: Users can set different methods being allowed for different locations, as well as for the root itself.
- **Authorization**: An authorization can be set with the login and password as a security measure.
- **Redirection**: A 301 and 302 redirect can be set for a permanent and temproral change of locations.
- **Autoindex**: An autoindex can be turned on and off for setting default page display.
- **CGI management**: Different CGI extensions can be allowed or restricted for all the locations being set.
- **Server name display**: Server domain names are being stored and displayed for a better server management.
</details>

<details>
  <summary>4. Logging and Monitoring</summary>
  Comprehensive logging and monitoring capabilities are built into our webserv project, providing valuable insights into server performance and client interactions:

#### **Access Logs**
The server maintains detailed access logs, recording each request received, including the method, URL, response status, and client IP address. This helps in tracking usage patterns and diagnosing issues.

#### **Error Logs**
Error logs capture any server-side errors, including failed requests and internal server errors. This information is crucial for debugging and improving server reliability.

#### **Server creation Logs**
After being created, a list of available servers is shown for an ease and understandable access and connection.
</details>

<details>
  <summary>5. Static and Dynamic Content Handling</summary>
  Our webserv project is capable of serving both static and dynamic content, making it versatile for various types of web applications:

#### **Static Content**
The server efficiently serves static files such as HTML, CSS, JavaScript, images, and other resources. It supports features like caching and compression to enhance delivery speed.

#### **Dynamic Content**
For dynamic content generation, the server can interface with backend scripts or applications written in languages like PHP, Python, or Bash. This allows for the creation of interactive and dynamic web applications.
</details>

## COLLABORATORS

Big shoutout to my collaborators [Drey Oduwole](https://github.com/DreyWesson), [Marvellous Oduwole](https://github.com/codedmarve) and [Nikita Ovsiannik](https://github.com/novsiannn) for a tremendous job they did on this project.

