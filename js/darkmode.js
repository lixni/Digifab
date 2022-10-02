/*const checkbox = document.getElementById('checkbox');

checkbox.addEventListener('change', ()=>{
  document.body.classList.toggle('dark');

})
*/


/*document.body.style = "background-color: var(--bs-dark);transition: 0.5s;";
const sun =
  "https://www.uplooder.net/img/image/55/7aa9993fc291bc170abea048589896cf/sun.svg";
const moon =
  "https://www.uplooder.net/img/image/2/addf703a24a12d030968858e0879b11e/moon.svg";

var theme = "dark";

const container = document.getElementsByClassName("theme-container")[0];
const themeIcon = document.getElementById("theme-icon");
container.addEventListener("click", setTheme);
function setTheme() {
  switch (theme) {
    case "dark":
      setLight();
      theme = "light";
      break;
    case "light":
      setDark();
      theme = "dark";
      break;
  }
}
function setLight() {

  container.classList.remove("shadow-dark");
  setTimeout(() => {
    document.body.classList.toggle('light');
    themeIcon.classList.remove("change");
  }, 300);
  themeIcon.classList.add("change");
  themeIcon.src = sun;
}
function setDark() {
  container.classList.remove("shadow-light");
  setTimeout(() => {
    document.body.classList.toggle('dark');
    themeIcon.classList.remove("change");
  }, 300);
  themeIcon.classList.add("change");
  themeIcon.src = moon;
}
*/



    document.getElementById("toggle").addEventListener("click", function()
{
    document.getElementsByTagName('body')[0].classList.toggle("dark-theme");
});