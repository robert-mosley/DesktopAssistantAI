# AI Assistant – Multi-Modal Productivity System

A personal AI assistant that lives on your desktop built to assist with tasks, research, and productivity.
---

## Prerequisites

- [Docker](https://www.docker.com/get-started) installed  
- Qt 6 (for GUI frontend)  
- Download and place the following in your repo:
  - **Whisper CLI** → `bin/whisper-cli.exe`  
  - **Whisper model** → `models/ggml-base.en.bin`

### Installation & Running

1. Clone the repository:  
```bash
git clone https://github.com/robert-mosley/desktopassistantai.git
cd ai-assistant
```
2. Build the Docker image:
```
docker build -t ai-assistant .
```

3. Run the container:
```
docker run -it --env-file .env -p 8000:8000 ai-assistant
```
#Usage

Start the Qt frontend (TextBox) to interact via voice or text.

Press the microphone button or type your query and hit Enter.

Responses appear in GUI windows and are optionally spoken via TTS.
