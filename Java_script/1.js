function bla(){
    console.log("Olá mundo perdido!")
}
b=document.querySelector("button:nth-child(4)");
b.innerHTML="Click em mim"
b.addEventListener("click", bla)