from urllib.parse import urlparse


def parse(log_file):
    commands = {}
    time = 0
    prev_time = None
    with open(log_file) as f:
        while True:
            line = f.readline()
            if not line:
                break

            line = line.strip()
            line_con = line.split(' ')
            if len(line_con) <= 0:
                continue
            if line_con[0] == "REST":
                rest_method = line_con[1]
                rest_url = line_con[3]

                if rest_url[len(rest_url) - 1] == ',':
                    rest_url = rest_url[:-1]
                parsed_rest_url = urlparse(rest_url)

                if parsed_rest_url.port != 9080:
                    continue

                host = parsed_rest_url.netloc.split(':')[0]

                path = parsed_rest_url.path.split('/')
                command_type = path[1]

                command = {"host": host, "type": command_type}

                if command["type"] == "receive" \
                        or command["type"] == "send" \
                        or command["type"] == "updateRate" \
                        or command["type"] == "progress":
                    command["id"] = path[2]

                for field in parsed_rest_url.query.split('&'):
                    if len(field) == 0:
                        continue
                    command[field.split('=')[0]] = field.split('=')[1]
                if not commands.get(time, None):
                    commands[time] = []
                commands[time].append(command)
            if line_con[0] == "***":
                prev_time = time
                time = int(float(line_con[3]))
                if time < prev_time:
                    break


    return commands
