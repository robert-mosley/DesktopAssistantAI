AI Assistant – Multi-Modal Productivity System

A modular AI assistant that integrates voice recognition, text-to-speech, and large language models to assist with tasks, research, and productivity.

Quick Start
Prerequisites

Docker installed

Optional: API keys for web search (TAVILY_API_KEY)

Installation & Running

Clone the repository:

git clone https://github.com/robert-mosley/ai-assistant.git
cd ai-assistant


Build the Docker image:

docker build -t ai-assistant .


Run the container:

docker run -it --env-file .env -p 8000:8000 ai-assistant


.env file should contain any required API keys, e.g., TAVILY_API_KEY=your_key_here.

Usage

Start the Qt frontend (TextBox) to interact via voice or text.

Press the microphone button or type your query and hit Enter.

Responses appear in GUI windows and are optionally spoken via TTS.
