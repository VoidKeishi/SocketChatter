FROM ubuntu:jammy

ENV TZ=Asia/Ho_Chi_Minh

# Update package list
RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y curl wget && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install build tools and pkg-config
RUN apt-get update && \
    apt-get install -y build-essential cmake git pkg-config && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Add Qt repository and key
RUN apt-get update && apt-get install -y software-properties-common && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install Qt 6 and related packages
RUN apt-get update && apt-get install -y \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-quick3d-dev \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qt6-l10n-tools \
    qt6-translations-l10n \
    qt6-documentation-tools \
    qml6-module-qtquick \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-layouts \
    qml6-module-qtquick-window \
    qml6-module-qtqml-models \
    qml6-module-qt-labs-platform \
    qml6-module-qtcore \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Additional dependencies for Qt Quick
RUN apt-get update && apt-get install -y \
    libgl1-mesa-dev \
    libvulkan-dev \
    libxcb-xinerama0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install Zsh
RUN apt-get update && apt-get install -y zsh && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* \
    source ~/.zshrc

# Install Oh My Zsh
# Uses "Spaceship" theme with some customization. Uses some bundled plugins and installs some more from github
RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.2.1/zsh-in-docker.sh)" -- \
    -t https://github.com/denysdovhan/spaceship-prompt \
    -a 'SPACESHIP_PROMPT_ADD_NEWLINE="false"' \
    -a 'SPACESHIP_PROMPT_SEPARATE_LINE="false"' \
    -p git \
    -p https://github.com/zsh-users/zsh-autosuggestions \
    -p https://github.com/zsh-users/zsh-completions