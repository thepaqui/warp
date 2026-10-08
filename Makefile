# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/11 00:49:35 by thepaqui          #+#    #+#              #
#    Updated: 2026/10/08 18:20:23 by thepaqui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= libwarp.a

CC= g++-10

CCFLAGS= -Wall -Wextra -Werror -std=c++2a

INCLUDES= -I./include/

SRCS=	src/glad.cpp \
		src/window.cpp \
		src/UserInput.cpp \
		src/IlluminationModels.cpp \
		src/ShaderInfo.cpp \
		src/ShaderProgram.cpp \
		src/ShaderManager.cpp \
		src/Texture2D.cpp \
		src/VAO.cpp \
		src/VBO.cpp \
		src/Mesh.cpp \
		src/Model.cpp \
		src/Material.cpp \
		src/Object.cpp \
		src/PointLight.cpp \
		src/DirectionalLight.cpp \
		src/Spotlight.cpp \
		src/EBO.cpp \
		src/Transform.cpp \
		src/Camera.cpp \
		src/Scene.cpp \
		src/MtlLoader.cpp \
		src/ObjLoader.cpp

OBJS= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "🎉 "$(NAME)" created successfully!"

%.o : %.cpp
	@$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@
	@echo "✅ Compiled "$@" successfully!"

clean:
	@rm -rf $(OBJS)
	@echo "🧹 Cleaned all object files!"

fclean: clean
	@rm -rf $(NAME)
	@echo "🧹 Removed "$(NAME)" successfully!"

re: fclean all

.PHONY: all clean fclean re