FROM archlinux/base

RUN pacman -Syu --noconfirm && \
    pacman -S base-devel sudo git --noconfirm

RUN useradd build -m && \
    passwd -d build && \
    echo "build ALL=(ALL) ALL" >> /etc/sudoers && \
    sudo -u build bash -c \
        'cd ~ && \
         git clone https://aur.archlinux.org/cpprestsdk.git && \
        cd cpprestsdk && \
        makepkg -sic --noconfirm' && \
    userdel build --remove

WORKDIR /opt/fip_service
COPY ./flickr_interesting_photo /opt/fip_service/

EXPOSE 80

ENTRYPOINT [ "/opt/fip_service/flickr_interesting_photo" ]
CMD [ "$FLICKR_API_KEY" ]

