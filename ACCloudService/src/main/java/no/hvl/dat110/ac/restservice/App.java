package no.hvl.dat110.ac.restservice;

import static spark.Spark.after;
import static spark.Spark.get;
import static spark.Spark.port;
import static spark.Spark.put;
import static spark.Spark.post;
import static spark.Spark.delete;

import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;

/**
 * Hello world!
 *
 */
public class App {
	
	static AccessLog accesslog = null;
	static AccessCode accesscode = null;
	
	public static void main(String[] args) {

		if (args.length > 0) {
			port(Integer.parseInt(args[0]));
		} else {
			port(8080);
		}

		// objects for data stored in the service
		
		accesslog = new AccessLog();
		accesscode  = new AccessCode();
		
		after((req, res) -> {
  		  res.type("application/json");
  		});
		
		// for basic testing purposes
		get("/accessdevice/hello", (req, res) -> {
			
		 	Gson gson = new Gson();
		 	
		 	return gson.toJson("IoT Access Control Device");
		});
		
		// TODO: implement the routes required for the access control service
		// as per the HTTP/REST operations describined in the project description

		post("/accessdevice/log", (req, res) -> {

			Gson gson = new Gson();

			try {
				AccessMessage message = gson.fromJson(req.body(), AccessMessage.class);

				if (message == null || message.getMessage() == null || message.getMessage().equals("")) {
					return gson.toJson("Feil JSON-format på meldingen");
				}

				int id = accesslog.add(message.getMessage());

				return gson.toJson(accesslog.get(id));
				
			} catch (JsonSyntaxException e) {
				return gson.toJson("Feil JSON-format på meldingen");
			}
			
		});

		get("/accessdevice/log", (req, res) -> {
			
			return accesslog.toJson();
		});

		get("/accessdevice/log/*", (req, res) -> {

			Gson gson = new Gson();

			int id = 0;
			String[] params = req.splat();
			if (params != null && params.length > 0) {
				try {
					id = Integer.parseInt(params[0]);
				} catch (NumberFormatException e) {
					return gson.toJson("Feil id-parameter");
				}
			} else {
				//ingen id-tal
				return accesslog.toJson();
			}
			AccessEntry entry = accesslog.get(id);
			if (entry != null) {
				return gson.toJson(entry);
			} else {
				return gson.toJson("Ingen med denne id-en");
			}
		});

		put("/accessdevice/code", (req, res) -> {
			Gson gson = new Gson();
			
			try {
				AccessCode code = gson.fromJson(req.body(), AccessCode.class);

				if (code == null || code.getAccesscode() == null || code.getAccesscode().length != 2) {
					return gson.toJson("Feil JSON-format på koden");
				}

				accesscode.setAccesscode(code.getAccesscode());

				return gson.toJson(code);
			} catch (JsonSyntaxException e) {
				return gson.toJson("Feil JSON-format på koden");
			}
			
		});

		get("/accessdevice/code", (req, res) -> {
			Gson gson = new Gson();

			return gson.toJson(accesscode);
		});

		delete("/accessdevice/log", (req, res) -> {

			accesslog.clear();

			return accesslog.toJson();
		});
		
    }
    
}
